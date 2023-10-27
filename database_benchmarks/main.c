//
// Created by Timur Guliamov on 27.10.2023.
//

#include "database.h"
#include "database_create_table_request.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert_benchmark() {
  const char *file_name =
      "/Users/tplaymeow/itmo-low-level-programming-lab1/insert_benchmark.db";
  FILE *file = fopen(file_name, "rb+");
  if (file == NULL) {
    return;
  }

  struct database *database = database_create_and_init(file);
  if (database == NULL) {
    return;
  }

  struct database_create_table_request table_request =
      database_create_table_request_create("Table", 3);
  database_create_table_request_set(
      table_request, 0,
      (struct database_attribute){.name = "ATTR1",
                                  .type = DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set(
      table_request, 1,
      (struct database_attribute){.name = "ATTR2",
                                  .type = DATABASE_ATTRIBUTE_FLOATING_POINT});
  database_create_table_request_set(
      table_request, 2,
      (struct database_attribute){.name = "ATTR3",
                                  .type = DATABASE_ATTRIBUTE_STRING});

  const struct database_create_table_result create_table_result =
      database_create_table(database, table_request);
  if (!create_table_result.success) {
    return;
  }

  database_create_table_request_destroy(table_request);

  const struct database_get_table_result get_table_result =
      database_get_table_with_name(database, "Table");
  if (!get_table_result.success) {
    return;
  }

  struct database_insert_row_request insert_request =
      database_insert_row_request_create(get_table_result.table);
  database_insert_row_request_set(
      insert_request, 0, (union database_attribute_value){.integer = 1});
  database_insert_row_request_set(
      insert_request, 1,
      (union database_attribute_value){.floating_point = 2.5});
  database_insert_row_request_set(
      insert_request, 2, (union database_attribute_value){.string = "Hello"});

  for (int i = 0; i < 6000; i++) {
    clock_t begin = clock();
    database_insert_row(database, get_table_result.table, insert_request);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%d %f\n", i, time_spent);
  }
}

void select_benchmark() {
  const char *file_name =
      "/Users/tplaymeow/itmo-low-level-programming-lab1/select_benchmark.db";
  FILE *file = fopen(file_name, "rb+");
  if (file == NULL) {
    return;
  }

  struct database *database = database_create_and_init(file);
  if (database == NULL) {
    return;
  }

  struct database_create_table_request table_request =
      database_create_table_request_create("Table", 3);
  database_create_table_request_set(
      table_request, 0,
      (struct database_attribute){.name = "ATTR1",
                                  .type = DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set(
      table_request, 1,
      (struct database_attribute){.name = "ATTR2",
                                  .type = DATABASE_ATTRIBUTE_FLOATING_POINT});
  database_create_table_request_set(
      table_request, 2,
      (struct database_attribute){.name = "ATTR3",
                                  .type = DATABASE_ATTRIBUTE_STRING});

  const struct database_create_table_result create_table_result =
      database_create_table(database, table_request);
  if (!create_table_result.success) {
    return;
  }

  database_create_table_request_destroy(table_request);

  const struct database_get_table_result get_table_result =
      database_get_table_with_name(database, "Table");
  if (!get_table_result.success) {
    return;
  }

  for (int i = 0; i < 6000; i++) {
    struct database_insert_row_request insert_request =
        database_insert_row_request_create(get_table_result.table);
    database_insert_row_request_set(
        insert_request, 0, (union database_attribute_value){.integer = i});
    database_insert_row_request_set(
        insert_request, 1,
        (union database_attribute_value){.floating_point = 2.5});
    database_insert_row_request_set(
        insert_request, 2, (union database_attribute_value){.string = "Hello"});

    database_insert_row(database, get_table_result.table, insert_request);

    database_insert_row_request_destroy(insert_request);

    const struct database_where where_first = {
        .operation = DATABASE_WHERE_OPERATION_EQUAL,
        .attribute_position = 0,
        .value = 0};
    const struct database_where where_last = {
        .operation = DATABASE_WHERE_OPERATION_EQUAL,
        .attribute_position = 0,
        .value = i};

    clock_t begin = clock();
    database_select_row_first(database, get_table_result.table, where_first);
    database_select_row_first(database, get_table_result.table, where_last);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%d %f\n", i, time_spent);
  }
}

void remove_benchmark() {
  const char *file_name =
      "/Users/tplaymeow/itmo-low-level-programming-lab1/remove_benchmark.db";
  FILE *file = fopen(file_name, "rb+");
  if (file == NULL) {
    return;
  }

  struct database *database = database_create_and_init(file);
  if (database == NULL) {
    return;
  }

  struct database_create_table_request table_request =
      database_create_table_request_create("Table", 3);
  database_create_table_request_set(
      table_request, 0,
      (struct database_attribute){.name = "ATTR1",
                                  .type = DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set(
      table_request, 1,
      (struct database_attribute){.name = "ATTR2",
                                  .type = DATABASE_ATTRIBUTE_FLOATING_POINT});
  database_create_table_request_set(
      table_request, 2,
      (struct database_attribute){.name = "ATTR3",
                                  .type = DATABASE_ATTRIBUTE_STRING});

  const struct database_create_table_result create_table_result =
      database_create_table(database, table_request);
  if (!create_table_result.success) {
    return;
  }

  database_create_table_request_destroy(table_request);

  const struct database_get_table_result get_table_result =
      database_get_table_with_name(database, "Table");
  if (!get_table_result.success) {
    return;
  }

  struct database_insert_row_request insert_request =
      database_insert_row_request_create(get_table_result.table);
  database_insert_row_request_set(
      insert_request, 0, (union database_attribute_value){.integer = 0});
  database_insert_row_request_set(
      insert_request, 1,
      (union database_attribute_value){.floating_point = 2.5});
  database_insert_row_request_set(
      insert_request, 2, (union database_attribute_value){.string = "Hello"});

  for (int i = 0; i < 600; i++) {
    for (int j = 0; j < 10; j++) {
      database_insert_row(database, get_table_result.table, insert_request);
    }

    const struct database_select_row_result select_result =
        database_select_row_first(database, get_table_result.table,
                                  DATABASE_WHERE_ALWAYS);

    clock_t begin = clock();
    database_remove_row(database, select_result.row);
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%d %f\n", i * 9, time_spent);
  }

  database_insert_row_request_destroy(insert_request);
}

void remove_memory_benchmark() {
  const char *file_name = "/Users/tplaymeow/itmo-low-level-programming-lab1/"
                          "remove_memory_benchmark.db";
  FILE *file = fopen(file_name, "rb+");
  if (file == NULL) {
    return;
  }

  struct database *database = database_create_and_init(file);
  if (database == NULL) {
    return;
  }

  struct database_create_table_request table_request =
      database_create_table_request_create("Table", 3);
  database_create_table_request_set(
      table_request, 0,
      (struct database_attribute){.name = "ATTR1",
                                  .type = DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set(
      table_request, 1,
      (struct database_attribute){.name = "ATTR2",
                                  .type = DATABASE_ATTRIBUTE_FLOATING_POINT});
  database_create_table_request_set(
      table_request, 2,
      (struct database_attribute){.name = "ATTR3",
                                  .type = DATABASE_ATTRIBUTE_STRING});

  const struct database_create_table_result create_table_result =
      database_create_table(database, table_request);
  if (!create_table_result.success) {
    return;
  }

  database_create_table_request_destroy(table_request);

  const struct database_get_table_result get_table_result =
      database_get_table_with_name(database, "Table");
  if (!get_table_result.success) {
    return;
  }

  struct database_insert_row_request insert_request =
      database_insert_row_request_create(get_table_result.table);
  database_insert_row_request_set(
      insert_request, 0, (union database_attribute_value){.integer = 1});
  database_insert_row_request_set(
      insert_request, 1,
      (union database_attribute_value){.floating_point = 2.5});
  database_insert_row_request_set(
      insert_request, 2, (union database_attribute_value){.string = "Hello"});

  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < 6000; j++) {
      database_insert_row(database, get_table_result.table, insert_request);
    }
    for (int j = 0; j < 6000; j++) {
      const struct database_select_row_result select_result =
          database_select_row_first(database, get_table_result.table,
                                    DATABASE_WHERE_ALWAYS);
      assert(select_result.success);
      if (!select_result.success) {
        abort();
      }

      database_remove_row(database, select_result.row);
    }

    const struct database_select_row_result select_result =
        database_select_row_first(database, get_table_result.table,
                                  DATABASE_WHERE_ALWAYS);
    assert(!select_result.success);
    if (select_result.success) {
      abort();
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);

    printf("%d %lu\n", i, file_size);
  }
}

int main() {
  insert_benchmark();
  select_benchmark();
  remove_benchmark();
  remove_memory_benchmark();
}