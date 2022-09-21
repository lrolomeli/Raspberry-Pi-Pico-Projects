file(GLOB FILES servo_pwm.c servo_pwm.h)
add_library(servoPWM ${FILES})
target_link_libraries(servoPWM pwmDriver)
target_include_directories(bypassValve PUBLIC ./)