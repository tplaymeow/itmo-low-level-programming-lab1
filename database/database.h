//
// Created by Timur Guliamov on 10.09.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_H

#include "database_create_table_request.h"
#include "database_insert_row_request.h"
#include "database_row.h"
#include "database_table.h"
#include "database_where.h"
#include <stdbool.h>
#include <stdio.h>

struct database;

struct database_create_table_result {
  bool success;
};

struct database_get_table_result {
  bool success;
  struct database_table table;
};

struct database_drop_table_result {
  bool success;
};

struct database_insert_row_result {
  bool success;
};

struct database_select_row_result {
  bool success;
  struct database_row row;
};

struct database_remove_row_result {
  bool success;
};

struct database *database_init(FILE *file);
struct database *database_create_and_init(FILE *file);

void database_destroy(struct database *database);

struct database_create_table_result
database_create_table(struct database *database,
                      struct database_create_table_request request);

struct database_get_table_result
database_get_table_with_name(const struct database *database, const char *name);

struct database_drop_table_result
  database_drop_table(struct database *database, struct database_table table);

struct database_insert_row_result
database_insert_row(struct database *database, struct database_table table,
                    struct database_insert_row_request request);

struct database_select_row_result
database_select_row_first(const struct database *database,
                          struct database_table table,
                          struct database_where where);
struct database_select_row_result database_select_row_next(
    const struct database *database, struct database_table table,
    struct database_where where, struct database_row previous);

struct database_remove_row_result
database_remove_row(const struct database *database, struct database_row row);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_H
