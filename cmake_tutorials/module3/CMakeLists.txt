cmake_minimum_required(VERSION 3.0.0)

project(Calculator_Project VERSION 1.0.0)

add_subdirectory(lib)

add_executable(calculator main.cpp)

add_executable(duplicate_calculator main.cpp)

target_link_libraries(calculator PRIVATE math_lib print_lib)
target_link_libraries(duplicate_calculator PRIVATE math_lib print_lib)