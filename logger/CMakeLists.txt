cmake_minimum_required(VERSION 3.24)
project(logger C)

set(CMAKE_C_STANDARD 11)

add_library(logger
        logger.h logger.c)
add_sanitizers(logger)

include(../scripts/filename.cmake)
define_file_info(logger)
