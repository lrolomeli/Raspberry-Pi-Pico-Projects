file(GLOB FILES pwm_driver.c pwm_driver.h)
add_library(pwmDriver ${FILES})
target_link_libraries(pwmDriver pico_stdlib hardware_pwm hardware_clocks)
target_include_directories(pwmDriver PUBLIC ./)