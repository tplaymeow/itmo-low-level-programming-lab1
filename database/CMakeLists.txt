cmake_minimum_required(VERSION 3.24)
project(database C)

set(CMAKE_C_STANDARD 11)

add_library(database
        database_attribute.h
        database_create_table_request.h database_create_table_request.c
        database_attribute_type.h
        database_attributes.h database_attributes.c
        database.h database.c)

include(../scripts/filename.cmake)
define_file_info(database)