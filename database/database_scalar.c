#include "database_scalar.h"

struct database_scalar database_scalar_make_integer(int32_t value) {
  return (struct database_scalar){.type = DATABASE_SCALAR_INTEGER,
                                  .value = {.integer = value}};
}

struct database_scalar database_scalar_make_floating_point(float value) {
  return (struct database_scalar){.type = DATABASE_SCALAR_FLOATING_POINT,
                                  .value = {.floating_point = value}};
}

struct database_scalar database_scalar_make_boolean(bool value) {
  return (struct database_scalar){.type = DATABASE_SCALAR_BOOLEAN,
                                  .value = {.boolean = value}};
}

struct database_scalar database_scalar_make_string(const char *value) {
  return (struct database_scalar){.type = DATABASE_SCALAR_STRING,
                                  .value = {.string = value}};
}
