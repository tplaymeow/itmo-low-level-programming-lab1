#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_SCALAR_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_SCALAR_H

#include <stdbool.h>
#include <stdint.h>

enum database_scalar_type {
  DATABASE_SCALAR_INTEGER,
  DATABASE_SCALAR_FLOATING_POINT,
  DATABASE_SCALAR_BOOLEAN,
  DATABASE_SCALAR_STRING
};

union database_scalar_value {
  int32_t integer;
  float floating_point;
  bool boolean;
  const char *string;
};

struct database_scalar {
  enum database_scalar_type type;
  union database_scalar_value value;
};

struct database_scalar database_scalar_make_integer(int32_t value);

struct database_scalar database_scalar_make_floating_point(float value);

struct database_scalar database_scalar_make_boolean(bool value);

struct database_scalar database_scalar_make_string(const char *value);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_DATABASE_SCALAR_H
