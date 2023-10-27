//
// Created by Timur Guliamov on 27.10.2023.
//

#include "database_where.h"
#include "database_attribute.h"
#include <string.h>

bool database_where_is_satisfied(struct database_table table,
                                 union database_attribute_value *values,
                                 struct database_where where) {
  const struct database_attribute attribute =
      database_attributes_get(table.attributes, where.attribute_position);
  switch (where.operation) {
  case DATABASE_WHERE_OPERATION_ALWAYS:
    return true;
  case DATABASE_WHERE_OPERATION_EQUAL:
    switch (attribute.type) {
    case DATABASE_ATTRIBUTE_INTEGER:
      return values[where.attribute_position].integer == where.value.integer;
    case DATABASE_ATTRIBUTE_FLOATING_POINT:
      return values[where.attribute_position].floating_point ==
             where.value.floating_point;
    case DATABASE_ATTRIBUTE_BOOLEAN:
      return values[where.attribute_position].boolean == where.value.boolean;
    case DATABASE_ATTRIBUTE_STRING:
      return strcmp(values[where.attribute_position].string,
                    where.value.string) == 0;
    default:
      return false;
    }
  case DATABASE_WHERE_OPERATION_LESS:
    switch (attribute.type) {
    case DATABASE_ATTRIBUTE_INTEGER:
      return values[where.attribute_position].integer < where.value.integer;
    case DATABASE_ATTRIBUTE_FLOATING_POINT:
      return values[where.attribute_position].floating_point <
             where.value.floating_point;
    case DATABASE_ATTRIBUTE_BOOLEAN:
      return values[where.attribute_position].boolean < where.value.boolean;
    case DATABASE_ATTRIBUTE_STRING:
      return strcmp(values[where.attribute_position].string,
                    where.value.string) < 0;
    default:
      return false;
    }
  case DATABASE_WHERE_OPERATION_GREATER:
    switch (attribute.type) {
    case DATABASE_ATTRIBUTE_INTEGER:
      return values[where.attribute_position].integer > where.value.integer;
    case DATABASE_ATTRIBUTE_FLOATING_POINT:
      return values[where.attribute_position].floating_point >
             where.value.floating_point;
    case DATABASE_ATTRIBUTE_BOOLEAN:
      return values[where.attribute_position].boolean > where.value.boolean;
    case DATABASE_ATTRIBUTE_STRING:
      return strcmp(values[where.attribute_position].string,
                    where.value.string) > 0;
    default:
      return false;
    }
  default:
    return false;
  }
}