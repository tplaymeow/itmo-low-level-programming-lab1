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

static void print_stat(struct paging_pager *pager) {
  void *data1 = NULL;
  struct paging_read_result res1 =
      paging_read_first(pager, PAGING_TYPE_1, &data1);
  while (res1.success) {
    debug("PL: %" PRIu64 "; CF: %" PRIu64 "; CL: %" PRIu64 "; NF: %" PRIu64,
          res1.info.previous_last_page_number,
          res1.info.current_first_page_number,
          res1.info.current_last_page_number, res1.info.next_first_page_number);
    free(data1);
    res1 = paging_read_next(pager, res1.info, &data1);
  }

  void *data2 = NULL;
  struct paging_read_result res2 =
      paging_read_first(pager, PAGING_TYPE_2, &data2);
  while (res2.success) {
    debug("PL: %" PRIu64 "; CF: %" PRIu64 "; CL: %" PRIu64 "; NF: %" PRIu64,
          res2.info.previous_last_page_number,
          res2.info.current_first_page_number,
          res2.info.current_last_page_number, res2.info.next_first_page_number);
    free(data2);
    res2 = paging_read_next(pager, res2.info, &data2);
  }

  void *data3 = NULL;
  struct paging_read_result res3 =
      paging_read_first(pager, PAGING_TYPE_3, &data3);
  while (res3.success) {
    debug("PL: %" PRIu64 "; CF: %" PRIu64 "; CL: %" PRIu64 "; NF: %" PRIu64,
          res3.info.previous_last_page_number,
          res3.info.current_first_page_number,
          res3.info.current_last_page_number, res3.info.next_first_page_number);
    free(data3);
    res3 = paging_read_next(pager, res3.info, &data3);
  }
}

int main1(int argc, char **argv) {
  const char *file_name = argv[1];
  FILE *file = fopen(file_name, "rb+");

  struct paging_pager *pager = paging_pager_create_and_init(file);
  if (pager == NULL) {
    debug("Pager is null");
    return -1;
  }

  const char *text =
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB "
      "Hi Timur from DB  Hi Timur from DB  Hi Timur from DB  Hi Timur from DB ";
  const size_t text_size = strlen(text) + 1;

  const char *text2 = "Hello World";
  const size_t text2_size = strlen(text2) + 1;

  debug("Add elements:");

  paging_write(pager, PAGING_TYPE_1, text, text_size);
  paging_write(pager, PAGING_TYPE_1, text2, text2_size);
  paging_write(pager, PAGING_TYPE_1, text, text_size);

  paging_write(pager, PAGING_TYPE_2, text, text_size);

  paging_write(pager, PAGING_TYPE_3, text2, text2_size);
  paging_write(pager, PAGING_TYPE_3, text2, text2_size);
  paging_write(pager, PAGING_TYPE_3, text2, text2_size);

  print_stat(pager);

  debug("Remove elements:");

  // Remove second type 1
  {
    void *data = NULL;
    struct paging_read_result r1 =
        paging_read_first(pager, PAGING_TYPE_1, &data);
    free(data);
    struct paging_read_result r2 = paging_read_next(pager, r1.info, &data);
    free(data);
    paging_remove(pager, r2.info);
  }

  // Remove first type 2
  {
    void *data = NULL;
    struct paging_read_result r1 =
        paging_read_first(pager, PAGING_TYPE_2, &data);
    free(data);
    paging_remove(pager, r1.info);
  }

  print_stat(pager);

  debug("Add elements:");

  paging_write(pager, PAGING_TYPE_3, text, text_size);
  paging_write(pager, PAGING_TYPE_1, text2, text2_size);

  print_stat(pager);
  return 0;
}

int main(int argc, char **argv) {
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

  struct database_create_table_request request =
      database_create_table_request_create("Users", 4);

  const struct database_attribute name = {.name = "NAME",
                                          .type = DATABASE_ATTRIBUTE_STRING};
  database_create_table_request_set(request, 0, name);
  const struct database_attribute login = {.name = "LOGIN",
                                           .type = DATABASE_ATTRIBUTE_STRING};
  database_create_table_request_set(request, 1, login);
  const struct database_attribute password = {
      .name = "PASSWORD", .type = DATABASE_ATTRIBUTE_STRING};
  database_create_table_request_set(request, 2, password);
  const struct database_attribute age = {.name = "AGE",
                                         .type = DATABASE_ATTRIBUTE_INTEGER};
  database_create_table_request_set(request, 3, age);

  const struct database_create_table_result create_table_result =
      database_create_table(database, request);
  if (!create_table_result.success) {
    debug("DB create table failed");
    return EXIT_FAILURE;
  }

  const struct database_get_table_result get_table_result =
      database_get_table_with_name(database, "Users");
  if (!get_table_result.success) {
    debug("DB get table failed");
    return EXIT_FAILURE;
  }

  debug("Table name: %s", get_table_result.table.name);
  for (size_t i = 0; i < get_table_result.table.attributes.count; i++) {
    const struct database_attribute attribute =
        database_attributes_get(get_table_result.table.attributes, i);
    debug("Attribute %lu: %s", i, attribute.name);
  }
}