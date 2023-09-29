//
// Created by Timur Guliamov on 10.09.2023.
//

#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_ATTRIBUTE_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_ATTRIBUTE_H

#include "database_attribute_type.h"

struct database_attribute {
  const char *name;
  enum database_attribute_type type;
};

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_ATTRIBUTE_H
