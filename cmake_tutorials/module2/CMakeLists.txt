cmake_minimum_required(VERSION 3.0.0)

project(Calculator_Project VERSION 1.0.0)

add_library(math_lib addition.cpp division.cpp)
add_library(print_lib print_result.cpp)

add_executable(calculator main.cpp)

add_executable(duplicate_calculator main.cpp)

target_link_libraries(calculator PRIVATE math_lib print_lib)
target_link_libraries(duplicate_calculator PRIVATE math_lib print_lib)