#ifndef SERVO_PWM_H
#define SERVO_PWM_H

#include "pwm_driver.h"

#define ANGLE_2_DUTY_CYCLE (10000 / 120)

void servo_setup(void);

bool servo_set_angle(unsigned char angle);

bool servo_start(void);

void servo_stop(void);

#endif /* SERVO_PWM_H */