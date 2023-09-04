#ifndef ITMO_LOW_LEVEL_PROGRAMMING_LAB1_LOGGER_H
#define ITMO_LOW_LEVEL_PROGRAMMING_LAB1_LOGGER_H

enum logger_level { LOGGER_DEBUG = 0, LOGGER_INFO };

#define debug(...) logger_log(LOGGER_DEBUG, FILENAME, __LINE__, __VA_ARGS__)
#define info(...) logger_log(LOGGER_INFO, FILENAME, __LINE__, __VA_ARGS__)

void logger_log(enum logger_level level, const char *file, int line,
                const char *format, ...);

#endif // ITMO_LOW_LEVEL_PROGRAMMING_LAB1_LOGGER_H
