#include "pwm_driver2.h"

static const unsigned int GPIO0 = 0;
static unsigned int slice = 0;
static bool pwm_config_ok = false;
static bool pwm_started = true;
        

void uv_pwm_setup_slice(void);

void uv_init_pwm_driver_20ms_period(void)
{
    pwm_config cfg = pwm_get_default_config();

    uv_pwm_setup_slice();
	
	// Divide PWM clock Frequency of 125MHz by 250 to obtain 1 MHz Frequency
	// pwm_set_clkdiv(slice, 250.0f);
	pwm_config_set_clkdiv_int_frac(&cfg, 250, 0);
	
	// 10,000 cycles at 1 MHz Frequency = 10,000 x 1 uS = 10,000 uS = 10 mS of period
	// After count gets to 10k, the counter wrap back to zero and so it completes 2 ms period
    // pwm_set_wrap(slice, 20000);
    pwm_config_set_wrap(&cfg, 20000);

    pwm_set_chan_level(slice, PWM_CHAN_A, 10000);

    pwm_init(slice, &cfg, true);

    pwm_config_ok = true;

}

bool uv_pwm_set_duty_cycle(unsigned short duty_cycle)
{

    if(pwm_config_ok)
    {
        // Set channel A output high for one cycle before dropping
        pwm_set_chan_level(slice, PWM_CHAN_A, duty_cycle);
        return true;
    }
    else
    {
        // Failed Configuration of PWM hardware peripherals.
        return false;
    }
        
}

bool uv_pwm_start(void)
{
    if(pwm_config_ok)
    {
        // Set PWM running
        pwm_set_enabled(slice, true);
        pwm_started = true;
        return true;
    }
    else
    {
        // Failed Configuration of PWM hardware peripherals.
        return false;
    }

}

bool uv_pwm_stop(void)
{
    if(pwm_started)
    {
        // stop PWM
        pwm_set_enabled(slice, false);
        return true;

    }
    else
    {
        //PWM has not started
        return false;
    }

}

void uv_pwm_setup_slice(void)
{
    // Tell GPIO 0 they are allocated to the PWM
    gpio_set_function(GPIO0, GPIO_FUNC_PWM);

    // Find out which PWM slice is connected to GPIO 0 (it's slice 0)
    slice = pwm_gpio_to_slice_num(GPIO0);

}