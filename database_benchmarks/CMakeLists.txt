cmake_minimum_required(VERSION 3.24)
project(database_benchmarks C)

set(CMAKE_C_STANDARD 11)

add_executable(database_benchmarks
        main.c)
add_sanitizers(database_benchmarks)

include_directories(
        "../database"
        "../logger"
        "../paging")
target_link_libraries(database_benchmarks
        database
        logger
        paging)

include(../scripts/filename.cmake)
define_file_info(database_benchmarks)
