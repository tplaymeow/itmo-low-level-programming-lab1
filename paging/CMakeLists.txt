cmake_minimum_required(VERSION 3.24)
project(paging C)

set(CMAKE_C_STANDARD 11)

add_library(paging
        paging.h paging.c)

include(../scripts/filename.cmake)
define_file_info(paging)