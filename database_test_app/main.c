#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "logger.h"

void print_all_rows(struct database *database, struct database_table table,
                    struct database_where where) {
  for (size_t i = 0; i < table.attributes.count; i++) {
    printf("%-15s", database_attributes_get(table.attributes, i).name);
  }
  printf("\n");

  for (struct database_select_row_result select_result =
           database_select_row_first(database, table, where);
       select_result.success; select_result = database_select_row_next(
                                  database, table, where, select_result.row)) {
    for (size_t i = 0; i < table.attributes.count; i++) {
      const struct database_attribute attribute =
          database_attributes_get(table.attributes, i);
      const union database_attribute_value value =
          database_attribute_values_get(select_result.row.values, i);
      switch (attribute.type) {
      case DATABASE_ATTRIBUTE_INTEGER:
        printf("%-15" PRIi64, value.integer);
        break;
      case DATABASE_ATTRIBUTE_FLOATING_POINT:
        printf("%-15lf", value.floating_point);
        break;
      case DATABASE_ATTRIBUTE_BOOLEAN:
        printf("%-15s", value.boolean ? "true" : "false");
        break;
      case DATABASE_ATTRIBUTE_STRING:
        printf("%-15s", value.string);
        break;
      default:
        break;
      }
    }
    printf("\n");
  }
}

void print_all_joined(struct database *database,
                      struct database_table left_table,
                      struct database_table right_table,
                      struct database_where left_where,
                      struct database_where right_where,
                      struct database_join join) {
  for (size_t i = 0; i < left_table.attributes.count; i++) {
    printf("%-15s", database_attributes_get(left_table.attributes, i).name);
  }
  for (size_t i = 0; i < right_table.attributes.count; i++) {
    printf("%-15s", database_attributes_get(right_table.attributes, i).name);
  }
  printf("\n");

  for (struct database_select_join_result select_result =
           database_select_join_first(database, left_table, right_table,
                                      left_where, right_where, join);
       select_result.success;
       select_result = database_select_join_next(
           database, left_table, right_table, left_where, right_where, join,
           select_result.left_row, select_result.right_row)) {
    for (size_t i = 0; i < left_table.attributes.count; i++) {
      const struct database_attribute attribute =
          database_attributes_get(left_table.attributes, i);
      const union database_attribute_value value =
          database_attribute_values_get(select_result.left_row.values, i);
      switch (attribute.type) {
      case DATABASE_ATTRIBUTE_INTEGER:
        printf("%-15" PRIi64, value.integer);
        break;
      case DATABASE_ATTRIBUTE_FLOATING_POINT:
        printf("%-15lf", value.floating_point);
        break;
      case DATABASE_ATTRIBUTE_BOOLEAN:
        printf("%-15s", value.boolean ? "true" : "false");
        break;
      case DATABASE_ATTRIBUTE_STRING:
        printf("%-15s", value.string);
        break;
      default:
        break;
      }
    }
    for (size_t i = 0; i < right_table.attributes.count; i++) {
      const struct database_attribute attribute =
          database_attributes_get(right_table.attributes, i);
      const union database_attribute_value value =
          database_attribute_values_get(select_result.right_row.values, i);
      switch (attribute.type) {
      case DATABASE_ATTRIBUTE_INTEGER:
        printf("%-15" PRIi64, value.integer);
        break;
      case DATABASE_ATTRIBUTE_FLOATING_POINT:
        printf("%-15lf", value.floating_point);
        break;
      case DATABASE_ATTRIBUTE_BOOLEAN:
        printf("%-15s", value.boolean ? "true" : "false");
        break;
      case DATABASE_ATTRIBUTE_STRING:
        printf("%-15s", value.string);
        break;
      default:
        break;
      }
    }
    printf("\n");
  }
}

struct database_table create_users_table(struct database *database) {
  struct database_create_table_request request =
      database_create_table_request_create("Users", 4);
  database_create_table_request_set_attribute(
      request, 0,
      (struct database_attribute){"id", DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set_attribute(
      request, 1,
      (struct database_attribute){"username", DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request, 2,
      (struct database_attribute){"password", DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request, 3,
      (struct database_attribute){"rating", DATABASE_ATTRIBUTE_FLOATING_POINT});
  database_create_table(database, request);
  database_create_table_request_destroy(request);
  return database_get_table_with_name(database, "Users").table;
}

void insert_user(struct database *database, struct database_table table,
                 int64_t id, const char *username, const char *password,
                 double rating) {
  struct database_insert_row_request request =
      database_insert_row_request_create(table);
  database_insert_row_request_set_value(
      request, 0, (union database_attribute_value){.integer = id});
  database_insert_row_request_set_value(
      request, 1, (union database_attribute_value){.string = username});
  database_insert_row_request_set_value(
      request, 2, (union database_attribute_value){.string = password});
  database_insert_row_request_set_value(
      request, 3, (union database_attribute_value){.floating_point = rating});
  database_insert_row(database, table, request);
  database_insert_row_request_destroy(request);
}

struct database_table create_posts_table(struct database *database) {
  struct database_create_table_request request =
      database_create_table_request_create("Posts", 5);
  database_create_table_request_set_attribute(
      request, 0,
      (struct database_attribute){"id", DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set_attribute(
      request, 1,
      (struct database_attribute){"timestamp", DATABASE_ATTRIBUTE_INTEGER});
  database_create_table_request_set_attribute(
      request, 2,
      (struct database_attribute){"title", DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request, 3,
      (struct database_attribute){"message", DATABASE_ATTRIBUTE_STRING});
  database_create_table_request_set_attribute(
      request, 4,
      (struct database_attribute){"user_id", DATABASE_ATTRIBUTE_INTEGER});
  database_create_table(database, request);
  database_create_table_request_destroy(request);
  return database_get_table_with_name(database, "Posts").table;
}

void insert_post(struct database *database, struct database_table table,
                 int64_t id, int64_t timestamp, const char *title,
                 const char *message, int64_t user_id) {
  struct database_insert_row_request request =
      database_insert_row_request_create(table);
  database_insert_row_request_set_value(
      request, 0, (union database_attribute_value){.integer = id});
  database_insert_row_request_set_value(
      request, 1, (union database_attribute_value){.integer = timestamp});
  database_insert_row_request_set_value(
      request, 2, (union database_attribute_value){.string = title});
  database_insert_row_request_set_value(
      request, 3, (union database_attribute_value){.string = message});
  database_insert_row_request_set_value(
      request, 4, (union database_attribute_value){.integer = user_id});
  database_insert_row(database, table, request);
  database_insert_row_request_destroy(request);
}

int test1(int argc, char **argv) {
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

  struct database_table users_table = create_users_table(database);

  insert_user(database, users_table, 0, "Timur0", "12345", 1.5);
  insert_user(database, users_table, 1, "Timur1", "12345", 2.5);
  insert_user(database, users_table, 2, "Timur2", "12345", 3.5);
  insert_user(database, users_table, 3, "Timur3", "12345", 4.5);
  insert_user(database, users_table, 4, "Timur4", "12345", 1.5);
  insert_user(database, users_table, 5, "Timur5", "12345", 2.5);
  insert_user(database, users_table, 6, "Timur6", "12345", 3.5);
  insert_user(database, users_table, 7, "Timur7", "12345", 4.5);

  printf("\nSelecting all rows\n");
  print_all_rows(database, users_table, DATABASE_WHERE_ALWAYS);

  printf("\nSelecting all rows with id > 4\n");
  print_all_rows(database, users_table,
                 (struct database_where){
                     .attribute_position = 0,
                     .operation = DATABASE_WHERE_OPERATION_GREATER,
                     .value = (union database_attribute_value){.integer = 4}});

  printf("\nSelecting all rows with username = Timur3\n");
  print_all_rows(
      database, users_table,
      (struct database_where){
          .attribute_position = 1,
          .operation = DATABASE_WHERE_OPERATION_EQUAL,
          .value = (union database_attribute_value){.string = "Timur3"}});

  printf("\nSelecting all rows with username = Timur\n");
  print_all_rows(
      database, users_table,
      (struct database_where){
          .attribute_position = 1,
          .operation = DATABASE_WHERE_OPERATION_EQUAL,
          .value = (union database_attribute_value){.string = "Timur"}});

  printf("\nRemoving row with username = Timur3\n");
  {
    struct database_select_row_result select_result = database_select_row_first(
        database, users_table,
        (struct database_where){
            .attribute_position = 1,
            .operation = DATABASE_WHERE_OPERATION_EQUAL,
            .value = (union database_attribute_value){.string = "Timur3"}});
    database_remove_row(database, select_result.row);
  }

  printf("\nRemoving row with id = 6\n");
  {
    struct database_select_row_result select_result = database_select_row_first(
        database, users_table,
        (struct database_where){
            .attribute_position = 0,
            .operation = DATABASE_WHERE_OPERATION_EQUAL,
            .value = (union database_attribute_value){.integer = 6}});
    database_remove_row(database, select_result.row);
  }

  printf("\nSelecting all rows\n");
  print_all_rows(database, users_table, DATABASE_WHERE_ALWAYS);

  const struct database_table posts_table = create_posts_table(database);

  insert_post(database, posts_table, 0, 1001, "Title 1", "Message 1", 0);
  insert_post(database, posts_table, 1, 1002, "Title 2", "Message 2", 1);
  insert_post(database, posts_table, 2, 1003, "Title 3", "Message 3", 2);
  insert_post(database, posts_table, 3, 1004, "Title 4", "Message 4", 4);
  insert_post(database, posts_table, 4, 1005, "Title 5", "Message 5", 5);
  insert_post(database, posts_table, 5, 1006, "Title 6", "Message 6", 7);
  insert_post(database, posts_table, 6, 1007, "Title 7", "Message 7", 1);
  insert_post(database, posts_table, 7, 1008, "Title 8", "Message 8", 1);
  insert_post(database, posts_table, 8, 1009, "Title 9", "Message 9", 1);
  insert_post(database, posts_table, 9, 1010, "Title 10", "Message 10", 7);
  insert_post(database, posts_table, 10, 1011, "Title 11", "Message 11", 4);
  insert_post(database, posts_table, 11, 1012, "Title 12", "Message 12", 5);
  insert_post(database, posts_table, 12, 1013, "Title 13", "Message 13", 5);
  insert_post(database, posts_table, 13, 1014, "Title 14", "Message 14", 5);
  insert_post(database, posts_table, 14, 1015, "Title 15", "Message 15", 0);
  insert_post(database, posts_table, 15, 1016, "Title 16", "Message 16", 0);
  insert_post(database, posts_table, 16, 1017, "Title 17", "Message 17", 7);

  printf("\nSelecting all rows\n");
  print_all_rows(database, posts_table, DATABASE_WHERE_ALWAYS);

  printf("\nSelecting all users joined with posts\n");
  print_all_joined(database, users_table, posts_table, DATABASE_WHERE_ALWAYS,
                   DATABASE_WHERE_ALWAYS,
                   (struct database_join){.left_attribute_position = 0,
                                          .right_attribute_position = 4});

  return EXIT_SUCCESS;
}

int main(int argc, char **argv) { return test1(argc, argv); }