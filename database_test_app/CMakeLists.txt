cmake_minimum_required(VERSION 3.24)
project(database_test_app C)

set(CMAKE_C_STANDARD 11)

add_executable(database_test_app
        main.c)
add_sanitizers(database_test_app)

include_directories(
        "../database"
        "../logger"
        "../paging")
target_link_libraries(database_test_app
        database
        logger
        paging)

include(../scripts/filename.cmake)
define_file_info(database_test_app)
