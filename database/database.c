//
// Created by Timur Guliamov on 10.09.2023.
//

#include "database.h"
#include "logger.h"
#include "paging.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct database {
  struct paging_pager *pager;
};

struct database_file_table_header {
  uint64_t table_name_offset;
  uint64_t attributes_count;
};

struct database_file_table_attribute {
  uint64_t attribute_name_offset;
  uint64_t attribute_type;
};

struct database_file_row_header {
  uint64_t table_name_offset;
};

struct database *database_init(FILE *file) {
  struct database *database = malloc(sizeof(struct database));
  if (database == NULL) {
    return NULL;
  }

  struct paging_pager *pager = paging_pager_init(file);
  if (pager == NULL) {
    return NULL;
  }

  database->pager = pager;
  return database;
}

struct database *database_create_and_init(FILE *file) {
  struct database *database = malloc(sizeof(struct database));
  if (database == NULL) {
    return NULL;
  }

  struct paging_pager *pager = paging_pager_create_and_init(file);
  if (pager == NULL) {
    return NULL;
  }

  database->pager = pager;
  return database;
}

void database_destroy(struct database *database) {
  if (database == NULL) {
    return;
  }
  paging_pager_destroy(database->pager);
  free(database);
}

static uint64_t database_attribute_type_to_uint64[] = {
    [DATABASE_ATTRIBUTE_INTEGER] = 0,
    [DATABASE_ATTRIBUTE_FLOATING_POINT] = 1,
    [DATABASE_ATTRIBUTE_BOOLEAN] = 2,
    [DATABASE_ATTRIBUTE_STRING] = 3,
};

static enum database_attribute_type database_attribute_type_from_uint64[] = {
    [0] = DATABASE_ATTRIBUTE_INTEGER,
    [1] = DATABASE_ATTRIBUTE_FLOATING_POINT,
    [2] = DATABASE_ATTRIBUTE_BOOLEAN,
    [3] = DATABASE_ATTRIBUTE_STRING,
};

struct database_create_table_result
database_create_table(struct database *database,
                      struct database_create_table_request request) {
  if (database == NULL) {
    return (struct database_create_table_result){.success = false};
  }

  const size_t header_size = sizeof(struct database_file_table_header);
  const size_t attribute_size = sizeof(struct database_file_table_attribute);
  const size_t data_size_without_strings =
      header_size + request.attributes.count * attribute_size;

  const size_t table_name_data_size = strlen(request.name) + 1;
  size_t strings_data_size = table_name_data_size;
  for (size_t i = 0; i < request.attributes.count; ++i) {
    strings_data_size +=
        strlen(database_attributes_get(request.attributes, i).name) + 1;
  }

  const size_t data_size = data_size_without_strings + strings_data_size;
  void *data = malloc(data_size);
  if (data == NULL) {
    debug("Alloc data error");
    return (struct database_create_table_result){.success = false};
  }

  size_t data_offset = 0;
  size_t data_strings_offset = data_size_without_strings;

  const struct database_file_table_header header = {
      .attributes_count = request.attributes.count,
      .table_name_offset = data_strings_offset};
  memcpy((char *)data + data_offset, &header, header_size);
  memcpy((char *)data + data_strings_offset, request.name,
         table_name_data_size);
  data_offset += header_size;
  data_strings_offset += table_name_data_size;

  for (size_t i = 0; i < request.attributes.count; ++i) {
    const struct database_attribute attribute =
        database_attributes_get(request.attributes, i);
    const struct database_file_table_attribute file_attribute = {
        .attribute_name_offset = data_strings_offset,
        .attribute_type = database_attribute_type_to_uint64[attribute.type]};
    const size_t attribute_name_size = strlen(attribute.name) + 1;
    memcpy((char *)data + data_offset, &file_attribute, attribute_size);
    memcpy((char *)data + data_strings_offset, attribute.name,
           attribute_name_size);
    data_offset += attribute_size;
    data_strings_offset += attribute_name_size;
  }

  assert(data_offset == data_size_without_strings);
  assert(data_strings_offset == data_size);

  struct paging_write_result write_result =
      paging_write(database->pager, PAGING_TYPE_1, data, data_size);
  if (!write_result.success) {
    debug("Write data to pager error");
    free(data);
    return (struct database_create_table_result){.success = false};
  }

  free(data);

  return (struct database_create_table_result){.success = true};
}

static struct database_table
database_table_from_file_data(struct paging_info page_info, void *data) {
  const struct database_file_table_header *header = data;
  char *table_name = (char *)data + header->table_name_offset;

  const struct database_file_table_attribute *file_attributes =
      (struct database_file_table_attribute
           *)((char *)data + sizeof(struct database_file_table_header));

  struct database_attributes attributes =
      database_attributes_create(header->attributes_count);
  for (size_t i = 0; i < header->attributes_count; ++i) {
    const struct database_attribute attribute = {
        .name = (char *)data + file_attributes[i].attribute_name_offset,
        .type = database_attribute_type_from_uint64[file_attributes[i]
                                                        .attribute_type]};
    database_attributes_set(attributes, i, attribute);
  }

  return (struct database_table){.data = data,
                                 .name = table_name,
                                 .page_info = page_info,
                                 .attributes = attributes};
}

struct database_get_table_result
database_get_table_with_name(const struct database *database,
                             const char *name) {
  if (database == NULL) {
    return (struct database_get_table_result){.success = false};
  }

  void *data = NULL;
  struct paging_read_result read_result =
      paging_read_first(database->pager, PAGING_TYPE_1, &data);

  while (read_result.success) {
    const struct database_table table =
        database_table_from_file_data(read_result.info, data);
    if (strcmp(table.name, name) == 0) {
      return (struct database_get_table_result){.success = true,
                                                .table = table};
    }

    free(data);
    read_result = paging_read_next(database->pager, read_result.info, &data);
  }

  return (struct database_get_table_result){.success = false};
}

struct database_drop_table_result
database_drop_table_result(struct database *database,
                           struct database_table table) {
  struct database_select_row_result select_result =
      database_select_row_first(database, table, DATABASE_WHERE_ALWAYS);
  while (select_result.success) {
    const struct database_remove_row_result remove_row_result =
        database_remove_row(database, select_result.row);
    if (!remove_row_result.success) {
      debug("Row removing error");
      return (struct database_drop_table_result){.success = false};
    }

    database_row_destroy(select_result.row);
    select_result =
        database_select_row_first(database, table, DATABASE_WHERE_ALWAYS);
  }

  const struct paging_remove_result remove_table_result =
      paging_remove(database->pager, table.page_info);
  if (!remove_table_result.success) {
    debug("Table removing error");
    return (struct database_drop_table_result){.success = false};
  }

  database_table_destroy(table);

  return (struct database_drop_table_result){.success = true};
}

struct database_insert_row_result
database_insert_row(struct database *database, struct database_table table,
                    struct database_insert_row_request request) {
  const size_t header_data_size = sizeof(struct database_file_row_header);
  const size_t integer_data_size = sizeof(int64_t);
  const size_t floating_point_data_size = sizeof(double);
  const size_t boolean_data_size = sizeof(uint64_t);
  const size_t table_name_data_size = strlen(table.name) + 1;

  size_t data_size_without_strings = header_data_size;
  size_t strings_data_size = table_name_data_size;
  for (size_t i = 0; i < table.attributes.count; i++) {
    switch (database_attributes_get(table.attributes, i).type) {
    case DATABASE_ATTRIBUTE_INTEGER:
      data_size_without_strings += integer_data_size;
      break;
    case DATABASE_ATTRIBUTE_FLOATING_POINT:
      data_size_without_strings += floating_point_data_size;
      break;
    case DATABASE_ATTRIBUTE_BOOLEAN:
      data_size_without_strings += boolean_data_size;
      break;
    case DATABASE_ATTRIBUTE_STRING:
      data_size_without_strings += sizeof(uint64_t);
      strings_data_size +=
          strlen(database_insert_row_request_get(request, i).string) + 1;
      break;
    default:
      break;
    }
  }

  const size_t data_size = data_size_without_strings + strings_data_size;
  void *data = malloc(data_size);
  if (data == NULL) {
    debug("Alloc data error");
    return (struct database_insert_row_result){.success = false};
  }

  size_t data_offset = 0;
  size_t data_strings_offset = data_size_without_strings;

  const struct database_file_row_header header = {.table_name_offset =
                                                      data_strings_offset};
  memcpy((char *)data + data_offset, &header, header_data_size);
  memcpy((char *)data + data_strings_offset, table.name, table_name_data_size);
  data_offset += header_data_size;
  data_strings_offset += table_name_data_size;

  for (size_t i = 0; i < table.attributes.count; i++) {
    switch (database_attributes_get(table.attributes, i).type) {
    case DATABASE_ATTRIBUTE_INTEGER: {
      const int64_t value = database_insert_row_request_get(request, i).integer;
      memcpy((char *)data + data_offset, &value, integer_data_size);
      data_offset += integer_data_size;
    } break;
    case DATABASE_ATTRIBUTE_FLOATING_POINT: {
      const double value =
          database_insert_row_request_get(request, i).floating_point;
      memcpy((char *)data + data_offset, &value, floating_point_data_size);
      data_offset += floating_point_data_size;
    } break;
    case DATABASE_ATTRIBUTE_BOOLEAN: {
      const uint64_t value =
          database_insert_row_request_get(request, i).boolean;
      memcpy((char *)data + data_offset, &value, boolean_data_size);
      data_offset += boolean_data_size;
    } break;
    case DATABASE_ATTRIBUTE_STRING: {
      const char *value = database_insert_row_request_get(request, i).string;
      const size_t string_data_size = strlen(value) + 1;
      memcpy((char *)data + data_offset, &data_strings_offset,
             sizeof(uint64_t));
      memcpy((char *)data + data_strings_offset, value, string_data_size);
      data_offset += sizeof(uint64_t);
      data_strings_offset += string_data_size;
    } break;
    default:
      break;
    }
  }

  assert(data_offset == data_size_without_strings);
  assert(data_strings_offset == data_size);

  struct paging_write_result write_result =
      paging_write(database->pager, PAGING_TYPE_2, data, data_size);
  if (!write_result.success) {
    debug("Write data to pager error");
    free(data);
    return (struct database_insert_row_result){.success = false};
  }

  free(data);

  return (struct database_insert_row_result){.success = true};
}

static union database_attribute_value *
database_row_values_from_file_data(struct database_table table,
                                   struct database_where where, void *data) {
  const size_t header_data_size = sizeof(struct database_file_row_header);
  const size_t integer_data_size = sizeof(int64_t);
  const size_t floating_point_data_size = sizeof(double);
  const size_t boolean_data_size = sizeof(uint64_t);

  size_t data_offset = 0;

  const struct database_file_row_header *header = data;
  data_offset += header_data_size;

  const char *table_name = (char *)data + header->table_name_offset;
  if (strcmp(table_name, table.name) != 0) {
    return NULL;
  }

  union database_attribute_value *values =
      malloc(table.attributes.count * sizeof(union database_attribute_value));
  if (values == NULL) {
    debug("Alloc values error");
    return NULL;
  }

  for (size_t i = 0; i < table.attributes.count; i++) {
    switch (database_attributes_get(table.attributes, i).type) {
    case DATABASE_ATTRIBUTE_INTEGER:
      values[i].integer = *((int64_t *)((char *)data + data_offset));
      data_offset += integer_data_size;
      break;
    case DATABASE_ATTRIBUTE_FLOATING_POINT:
      values[i].floating_point = *((double *)((char *)data + data_offset));
      data_offset += floating_point_data_size;
      break;
    case DATABASE_ATTRIBUTE_BOOLEAN:
      values[i].boolean = *((uint64_t *)((char *)data + data_offset));
      data_offset += boolean_data_size;
      break;
    case DATABASE_ATTRIBUTE_STRING: {
      const uint64_t string_offset =
          *((uint64_t *)((char *)data + data_offset));
      values[i].string = ((char *)data + string_offset);
      data_offset += sizeof(uint64_t);
    } break;
    default:
      break;
    }
  }

  if (!database_where_is_satisfied(table, values, where)) {
    free(values);
    return NULL;
  }

  return values;
}

struct database_select_row_result
database_select_row_first(const struct database *database,
                          struct database_table table,
                          struct database_where where) {
  if (database == NULL) {
    return (struct database_select_row_result){.success = false};
  }

  void *data = NULL;
  struct paging_read_result read_result =
      paging_read_first(database->pager, PAGING_TYPE_2, &data);

  while (read_result.success) {
    union database_attribute_value *values =
        database_row_values_from_file_data(table, where, data);
    if (values != NULL) {
      return (struct database_select_row_result){
          .success = true,
          .row = {
              .data = data, .paging_info = read_result.info, .values = values}};
    }

    free(data);
    read_result = paging_read_next(database->pager, read_result.info, &data);
  }

  return (struct database_select_row_result){.success = false};
}

struct database_select_row_result database_select_row_next(
    const struct database *database, struct database_table table,
    struct database_where where, struct database_row previous) {
  if (database == NULL) {
    return (struct database_select_row_result){.success = false};
  }

  void *data = NULL;
  struct paging_read_result read_result =
      paging_read_next(database->pager, previous.paging_info, &data);

  while (read_result.success) {
    union database_attribute_value *values =
        database_row_values_from_file_data(table, where, data);
    if (values != NULL) {
      return (struct database_select_row_result){
          .success = true,
          .row = {
              .data = data, .paging_info = read_result.info, .values = values}};
    }

    free(data);
    read_result = paging_read_next(database->pager, read_result.info, &data);
  }

  return (struct database_select_row_result){.success = false};
}

struct database_remove_row_result
database_remove_row(const struct database *database, struct database_row row) {
  if (database == NULL) {
    return (struct database_remove_row_result){.success = false};
  }

  const struct paging_remove_result result =
      paging_remove(database->pager, row.paging_info);
  if (!result.success) {
    debug("Remove row from pager error");
    return (struct database_remove_row_result){.success = false};
  }

  database_row_destroy(row);
  return (struct database_remove_row_result){.success = true};
}