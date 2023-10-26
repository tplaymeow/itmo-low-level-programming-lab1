//
// Created by Timur Guliamov on 26.10.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_INSERT_ROW_REQUEST_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_INSERT_ROW_REQUEST_H

#include "database_attribute_value.h"
#include "database_table.h"

struct database_insert_row_request {
  union database_attribute_value *values;
};

struct database_insert_row_request
database_insert_row_request_create(struct database_table table);

void database_insert_row_request_destroy(
    struct database_insert_row_request request);

void database_insert_row_request_set(struct database_insert_row_request request,
                                     size_t position,
                                     union database_attribute_value value);

union database_attribute_value
database_insert_row_request_get(struct database_insert_row_request request,
                                size_t position);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_INSERT_ROW_REQUEST_H
