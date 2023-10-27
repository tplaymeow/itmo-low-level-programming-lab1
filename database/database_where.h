//
// Created by Timur Guliamov on 27.10.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_WHERE_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_WHERE_H

#include "database_row.h"
#include "database_table.h"
#include <stddef.h>

#define DATABASE_WHERE_ALWAYS                                                  \
  ((struct database_where){.operation = DATABASE_WHERE_OPERATION_ALWAYS})

struct database_where {
  size_t attribute_position;
  union database_attribute_value value;
  enum {
    DATABASE_WHERE_OPERATION_ALWAYS,
    DATABASE_WHERE_OPERATION_EQUAL,
    DATABASE_WHERE_OPERATION_LESS,
    DATABASE_WHERE_OPERATION_GREATER,
  } operation;
};

bool database_where_is_satisfied(struct database_table table,
                                 union database_attribute_value *values,
                                 struct database_where where);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_WHERE_H
