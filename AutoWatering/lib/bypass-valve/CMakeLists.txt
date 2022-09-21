file(GLOB FILES bypass_valve.c bypass_valve.h)
add_library(bypassValve ${FILES})
target_link_libraries(bypassValve servoPWM)
target_include_directories(bypassValve PUBLIC ./)