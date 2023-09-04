cmake_minimum_required(VERSION 3.24)
project(logger C)

set(CMAKE_C_STANDARD 11)

add_library(logger
        logger.h logger.c)

include(../scripts/filename.cmake)
define_filename_for_sources(logger)
