cmake_minimum_required(VERSION 3.24)
project(utils C)

set(CMAKE_C_STANDARD 11)

add_library(utils
        math_utils.h
        math_utils.c
)

include(../scripts/filename.cmake)
define_file_info(utils)