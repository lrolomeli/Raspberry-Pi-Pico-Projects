file(GLOB FILES pwm_driver2.c pwm_driver2.h)
add_library(pwmDriver2 ${FILES})
target_link_libraries(pwmDriver2 pico_stdlib hardware_pwm hardware_clocks)
target_include_directories(pwmDriver2 PUBLIC ./)