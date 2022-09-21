# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(AutoWatering C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 1 Create an INTERFACE library aggregating all the common parts of the application
add_subdirectory(./lib/decoding-fsm)
add_subdirectory(./lib/bypass-valve)
add_subdirectory(./lib/servo-pwm)
add_subdirectory(./lib/pwm-driver)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} decodingFSM bypassValve)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)