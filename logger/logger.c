#include <stdarg.h>
#include <stdio.h>

#include "logger.h"

static const char *logger_level_text[] = {
    [LOGGER_DEBUG] = "DEBUG", [LOGGER_INFO] = "INFO"};

static void printf_varargs(const char *format, ...) {

}
void logger_log(enum logger_level level, const char *file, int line,
                const char *format, ...) {
  const char *level_text = logger_level_text[level];
  printf("[%s] %s %d: ", level_text, file, line);
  va_list arguments;
  va_start(arguments, format);
  vprintf(format, arguments);
  va_end(arguments);
  printf("\n");
}