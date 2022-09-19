#ifndef SERVO_PWM_H
#define SERVO_PWM_H

#include "pwm_driver.h"


void servo_setup(void);

bool servo_set_angle(unsigned int angle);

bool servo_start(void);

void servo_stop(void);

#endif /* SERVO_PWM_H */