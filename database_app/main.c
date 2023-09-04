#include <inttypes.h>
#include <stdio.h>

#include "database_scalar.h"
#include "logger.h"

int main(int argc, char **argv) {
  const struct database_scalar scalars[] = {
      database_scalar_make_integer(55),
      database_scalar_make_floating_point(0.333f),
      database_scalar_make_boolean(true),
      database_scalar_make_string("Hello"),
  };

  for (size_t i = 0; i < 4; i++) {
    const struct database_scalar scalar = scalars[i];
    switch (scalar.type) {
    case DATABASE_SCALAR_INTEGER:
      debug("%" PRId32, scalar.value.integer);
      break;
    case DATABASE_SCALAR_FLOATING_POINT:
      info("%f", scalar.value.floating_point);
      break;
    case DATABASE_SCALAR_BOOLEAN:
      debug(scalar.value.boolean ? "True" : "False");
      break;
    case DATABASE_SCALAR_STRING:
      info("%s", scalar.value.string);
      break;
    }
  }
}