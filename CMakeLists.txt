cmake_minimum_required(VERSION 3.24)
project(itmo_low_level_programming_lab1 C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(database_library)
add_subdirectory(database_app)

include(format.cmake)
