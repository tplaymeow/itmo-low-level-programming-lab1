//
// Created by Timur Guliamov on 26.10.2023.
//

#include "database_insert_row_request.h"
#include "database_attribute_value.h"
#include "database_table.h"
#include <stdlib.h>

struct database_insert_row_request
database_insert_row_request_create(struct database_table table) {
  union database_attribute_value *values =
      malloc(table.attributes.count * sizeof(union database_attribute_value));
  return (struct database_insert_row_request){.values = values};
}

void database_insert_row_request_destroy(
    struct database_insert_row_request request) {
  if (request.values == NULL) {
    return;
  }
  free(request.values);
}

void database_insert_row_request_set(struct database_insert_row_request request,
                                     size_t position,
                                     union database_attribute_value value) {
  request.values[position] = value;
}

union database_attribute_value
database_insert_row_request_get(struct database_insert_row_request request,
                                size_t position) {
  return request.values[position];
}