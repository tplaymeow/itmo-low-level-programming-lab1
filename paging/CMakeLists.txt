cmake_minimum_required(VERSION 3.24)
project(paging C)

set(CMAKE_C_STANDARD 11)

include_directories(
        "../utils"
        "../logger")
target_link_libraries(
        utils
        logger)

add_library(paging
        paging.h paging.c)
add_sanitizers(paging)


include(../scripts/filename.cmake)
define_file_info(paging)