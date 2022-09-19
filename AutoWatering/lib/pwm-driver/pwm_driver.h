#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include "hardware/clocks.h"
#include "hardware/pwm.h"


void uv_init_pwm_driver_20ms_period(void);

bool uv_pwm_set_duty_cycle(unsigned short duty_cycle);

bool uv_pwm_start(void);

bool uv_pwm_stop(void);


#endif /* PWM_DRIVER_H */