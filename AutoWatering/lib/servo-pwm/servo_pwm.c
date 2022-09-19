#include "servo_pwm.h"

void servo_setup(void)
{
    uv_init_pwm_driver_20ms_period();
}

bool servo_set_angle(unsigned int angle)
{
    unsigned int duty_cycle = 0;

    if(angle >= 0 && angle <= 120)
    {
        duty_cycle = map(angle, 0, 120, 0, 10000);
        return uv_pwm_set_duty_cycle(duty_cycle);
    }
    else
    {
        // Set a valid angle for this servo
        return false;
    }

}

bool servo_start(void)
{
    return uv_pwm_start();
}

void servo_stop(void)
{
    (void)uv_pwm_stop();
}
