cmake_minimum_required(VERSION 3.24)
project(database_app C)

set(CMAKE_C_STANDARD 11)

add_executable(database_app
        main.c)

include_directories("../database_library")
target_link_libraries(database_app database_library)
