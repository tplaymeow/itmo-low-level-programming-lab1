//
// Created by Timur Guliamov on 26.10.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_TABLE_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_TABLE_H

#include "database_attribute.h"
#include "database_attributes.h"

struct database_table {
  void *data;
  char *name;
  struct database_attributes attributes;
};

void database_table_destroy(struct database_table table);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_TABLE_H
