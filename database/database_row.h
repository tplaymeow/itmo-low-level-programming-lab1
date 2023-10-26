//
// Created by Timur Guliamov on 26.10.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_ROW_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_ROW_H

#include "database_attribute_value.h"
#include "paging.h"

struct database_row {
  void *data;
  struct paging_info paging_info;
  union database_attribute_value *values;
};

void database_row_destroy(struct database_row row);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_ROW_H
