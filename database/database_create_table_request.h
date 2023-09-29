//
// Created by Timur Guliamov on 10.09.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_CREATE_TABLE_REQUEST_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_CREATE_TABLE_REQUEST_H

#include "database_attribute.h"
#include "database_attributes.h"
#include <stddef.h>

struct database_create_table_request {
  const char *name;
  struct database_attributes attributes;
};

struct database_create_table_request
database_create_table_request_create(const char *table_name,
                                     size_t attributes_count);

void database_create_table_request_destroy(
    struct database_create_table_request table);

void database_create_table_request_set(
    struct database_create_table_request table, size_t position,
    struct database_attribute attribute);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_CREATE_TABLE_REQUEST_H
