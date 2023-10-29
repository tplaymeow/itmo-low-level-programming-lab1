cmake_minimum_required(VERSION 3.24)
project(itmo_low_level_programming_lab1 C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/externals/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)

add_subdirectory(utils)
add_subdirectory(logger)
add_subdirectory(paging)
add_subdirectory(database)
add_subdirectory(database_test_app)
add_subdirectory(database_benchmarks)

include(scripts/format.cmake)