#include "pwm_driver2.h"

int main()
{

    stdio_init_all();
    uv_init_pwm_driver_20ms_period();
    while(1)
    {
        sleep_ms(10);
    }

    return 0;
}