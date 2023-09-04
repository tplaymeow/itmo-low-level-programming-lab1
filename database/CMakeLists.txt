cmake_minimum_required(VERSION 3.24)
project(database C)

set(CMAKE_C_STANDARD 11)

add_library(database
        database_scalar.h database_scalar.c)

include(../scripts/filename.cmake)
define_file_info(database)