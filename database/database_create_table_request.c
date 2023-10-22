//
// Created by Timur Guliamov on 10.09.2023.
//

#include "database_create_table_request.h"

#include <stdlib.h>

#include "database_attribute.h"
#include "database_attributes.h"

struct database_create_table_request
database_create_table_request_create(const char *table_name,
                                     size_t attributes_count) {
  struct database_attributes attributes =
      database_attributes_create(attributes_count);
  return (struct database_create_table_request){.name = table_name,
                                                .attributes = attributes};
}

void database_create_table_request_destroy(
    struct database_create_table_request table) {
  database_attributes_destroy(table.attributes);
}

void database_create_table_request_set(
    struct database_create_table_request table, size_t position,
    struct database_attribute attribute) {
  database_attributes_set(table.attributes, position, attribute);
}
