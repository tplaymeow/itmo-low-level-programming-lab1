//
// Created by Timur Guliamov on 26.10.2023.
//

#include "database_table.h"
#include <stdlib.h>

void database_table_destroy(struct database_table table) {
  if (table.data) {
    free(table.data);
  }

  database_attributes_destroy(table.attributes);
}
