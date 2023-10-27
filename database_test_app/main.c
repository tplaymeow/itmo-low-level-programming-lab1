#include <assert.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "database.h"
#include "database_create_table_request.h"
#include "logger.h"
#include "paging.h"

//void print_all_selected_rows(struct database *database,
//                             struct database_table table,
//                             struct database_where where) {
//  struct database_select_row_result select_row_result =
//      database_select_row_first(database, table, where);
//  while (select_row_result.success) {
//    for (size_t i = 0; i < table.attributes.count; i++) {
//      const struct database_attribute attribute =
//          database_attributes_get(table.attributes, i);
//      const
//      const union database_attribute_value value = sel_res2.row.values[i];
//    }
//
//    const struct database_select_row_result select_row_result_new =
//        database_select_row_next(database, table, where, select_row_result.row);
//    database_row_destroy(select_row_result.row);
//    select_row_result = select_row_result_new;
//  }
//}

int test(int argc, char **argv) {
  FILE *file = fopen(argv[1], "rb+");
  if (file == NULL) {
    debug("File open failed");
    return EXIT_FAILURE;
  }

  struct database *database = database_create_and_init(file);
  if (database == NULL) {
    debug("DB init failed");
    return EXIT_FAILURE;
  }

  struct database_create_table_request request1 =
      database_create_table_request_create("Users", 4);

  database_create_table_request_set_attribute(
      request1, 0,
      (struct database_attribute){.name = "NAME",
                                  .type = DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request1, 1,
      (struct database_attribute){.name = "LOGIN",
                                  .type = DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request1, 2,
      (struct database_attribute){.name = "PASSWORD",
                                  .type = DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request1, 3,
      (struct database_attribute){.name = "AGE",
                                  .type = DATABASE_ATTRIBUTE_INTEGER});

  const struct database_create_table_result create_table_result1 =
      database_create_table(database, request1);
  if (!create_table_result1.success) {
    debug("DB create table failed");
    return EXIT_FAILURE;
  }

  struct database_create_table_request request2 =
      database_create_table_request_create("Table", 4);

  database_create_table_request_set_attribute(
      request2, 0,
      (struct database_attribute){.name = "INT",
                                  .type = DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set_attribute(
      request2, 1,
      (struct database_attribute){.name = "BOOL",
                                  .type = DATABASE_ATTRIBUTE_BOOLEAN});
  database_create_table_request_set_attribute(
      request2, 2,
      (struct database_attribute){.name = "STRING",
                                  .type = DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request2, 3,
      (struct database_attribute){.name = "DOUBLE",
                                  .type = DATABASE_ATTRIBUTE_FLOATING_POINT});

  const struct database_create_table_result create_table_result2 =
      database_create_table(database, request2);
  if (!create_table_result2.success) {
    debug("DB create table failed");
    return EXIT_FAILURE;
  }

  const struct database_get_table_result get_table_result1 =
      database_get_table_with_name(database, "Users");
  if (!get_table_result1.success) {
    debug("DB get table failed");
    return EXIT_FAILURE;
  }

  const struct database_get_table_result get_table_result2 =
      database_get_table_with_name(database, "Table");
  if (!get_table_result2.success) {
    debug("DB get table failed");
    return EXIT_FAILURE;
  }

  debug("Table name: %s", get_table_result1.table.name);
  for (size_t i = 0; i < get_table_result1.table.attributes.count; i++) {
    const struct database_attribute attribute =
        database_attributes_get(get_table_result1.table.attributes, i);
    debug("Attribute %lu: %s", i, attribute.name);
  }

  debug("Table name: %s", get_table_result2.table.name);
  for (size_t i = 0; i < get_table_result2.table.attributes.count; i++) {
    const struct database_attribute attribute =
        database_attributes_get(get_table_result2.table.attributes, i);
    debug("Attribute %lu: %s", i, attribute.name);
  }

  struct database_insert_row_request insert_request1 =
      database_insert_row_request_create(get_table_result1.table);
  database_insert_row_request_set_value(
      insert_request1, 0, (union database_attribute_value){.string = "Timur"});
  database_insert_row_request_set_value(
      insert_request1, 1,
      (union database_attribute_value){.string = "tplaymeow"});
  database_insert_row_request_set_value(
      insert_request1, 2, (union database_attribute_value){.string = "12345"});
  database_insert_row_request_set_value(
      insert_request1, 3, (union database_attribute_value){.integer = 21});

  database_insert_row(database, get_table_result1.table, insert_request1);
  database_insert_row(database, get_table_result1.table, insert_request1);
  database_insert_row(database, get_table_result1.table, insert_request1);
  database_insert_row(database, get_table_result1.table, insert_request1);

  database_insert_row_request_destroy(insert_request1);

  struct database_insert_row_request insert_request2 =
      database_insert_row_request_create(get_table_result2.table);
  database_insert_row_request_set_value(
      insert_request2, 0, (union database_attribute_value){.integer = 123});
  database_insert_row_request_set_value(
      insert_request2, 1, (union database_attribute_value){.boolean = true});
  database_insert_row_request_set_value(
      insert_request2, 2, (union database_attribute_value){.string = "12345"});
  database_insert_row_request_set_value(
      insert_request2, 3,
      (union database_attribute_value){.floating_point = 0.666});

  database_insert_row(database, get_table_result2.table, insert_request2);
  database_insert_row(database, get_table_result2.table, insert_request2);
  database_insert_row(database, get_table_result2.table, insert_request2);

  database_insert_row_request_destroy(insert_request2);

  struct database_insert_row_request insert_request3 =
      database_insert_row_request_create(get_table_result1.table);
  database_insert_row_request_set_value(
      insert_request3, 0, (union database_attribute_value){.string = "Gleb"});
  database_insert_row_request_set_value(
      insert_request3, 1, (union database_attribute_value){.string = "gleb"});
  database_insert_row_request_set_value(
      insert_request3, 2, (union database_attribute_value){.string = "67890"});
  database_insert_row_request_set_value(
      insert_request3, 3, (union database_attribute_value){.integer = 21});

  database_insert_row(database, get_table_result1.table, insert_request3);
  database_insert_row(database, get_table_result1.table, insert_request3);
  database_insert_row(database, get_table_result1.table, insert_request3);

  database_insert_row_request_destroy(insert_request3);

  struct database_insert_row_request insert_request4 =
      database_insert_row_request_create(get_table_result2.table);
  database_insert_row_request_set_value(
      insert_request4, 0, (union database_attribute_value){.integer = 321});
  database_insert_row_request_set_value(
      insert_request4, 1, (union database_attribute_value){.boolean = false});
  database_insert_row_request_set_value(
      insert_request4, 2, (union database_attribute_value){.string = "string"});
  database_insert_row_request_set_value(
      insert_request4, 3,
      (union database_attribute_value){.floating_point = 3.14});

  database_insert_row(database, get_table_result2.table, insert_request4);
  database_insert_row(database, get_table_result2.table, insert_request4);
  database_insert_row(database, get_table_result2.table, insert_request4);

  database_insert_row_request_destroy(insert_request4);

  struct database_where where = {.attribute_position = 0,
                                 .value = {.string = "Timur"},
                                 .operation = DATABASE_WHERE_OPERATION_EQUAL};
  struct database_select_row_result sel_res1 =
      database_select_row_first(database, get_table_result1.table, where);
  while (sel_res1.success) {
    database_remove_row(database, sel_res1.row);
    sel_res1 =
        database_select_row_first(database, get_table_result1.table, where);
  }

  sel_res1 = database_select_row_first(database, get_table_result1.table,
                                       DATABASE_WHERE_ALWAYS);
  while (sel_res1.success) {
    for (size_t i = 0; i < get_table_result1.table.attributes.count; i++) {
      const struct database_attribute attribute =
          database_attributes_get(get_table_result1.table.attributes, i);
      const union database_attribute_value value =
          database_attribute_values_get(sel_res1.row.values, i);
      if (attribute.type == DATABASE_ATTRIBUTE_STRING) {
        debug("%s: %s", attribute.name, value.string);
      } else if (attribute.type == DATABASE_ATTRIBUTE_INTEGER) {
        debug("%s: %" PRIi64, attribute.name, value.integer);
      } else if (attribute.type == DATABASE_ATTRIBUTE_FLOATING_POINT) {
        debug("%s: %lf", attribute.name, value.floating_point);
      } else if (attribute.type == DATABASE_ATTRIBUTE_BOOLEAN) {
        debug("%s: %s", attribute.name, value.boolean ? "true" : "false");
      }
    }
    debug("-------------------");
    sel_res1 = database_select_row_next(database, get_table_result1.table,
                                        DATABASE_WHERE_ALWAYS, sel_res1.row);
  }

  struct database_select_row_result sel_res2 = database_select_row_first(
      database, get_table_result2.table, DATABASE_WHERE_ALWAYS);
  while (sel_res2.success) {
    for (size_t i = 0; i < get_table_result2.table.attributes.count; i++) {
      const struct database_attribute attribute =
          database_attributes_get(get_table_result2.table.attributes, i);
      const union database_attribute_value value =
          database_attribute_values_get(sel_res2.row.values, i);
      if (attribute.type == DATABASE_ATTRIBUTE_STRING) {
        debug("%s: %s", attribute.name, value.string);
      } else if (attribute.type == DATABASE_ATTRIBUTE_INTEGER) {
        debug("%s: %" PRIi64, attribute.name, value.integer);
      } else if (attribute.type == DATABASE_ATTRIBUTE_FLOATING_POINT) {
        debug("%s: %lf", attribute.name, value.floating_point);
      } else if (attribute.type == DATABASE_ATTRIBUTE_BOOLEAN) {
        debug("%s: %s", attribute.name, value.boolean ? "true" : "false");
      }
    }
    debug("-------------------");
    sel_res2 = database_select_row_next(database, get_table_result2.table,
                                        DATABASE_WHERE_ALWAYS, sel_res2.row);
  }
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) { test(argc, argv); }