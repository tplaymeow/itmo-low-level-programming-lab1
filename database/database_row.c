//
// Created by Timur Guliamov on 26.10.2023.
//

#include "database_row.h"
#include <stdlib.h>

void database_row_destroy(struct database_row row) {
  if (row.data) {
    free(row.data);
  }

  database_attribute_values_destroy(row.values);
}