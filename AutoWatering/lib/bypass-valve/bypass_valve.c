#include "bypass_valve.h"

void open_bp_valve(void)
{
    if(servo_set_angle(BP_FULLY_OPEN_VALVE_ANGLE))
    {
        // printf("Valve is fully open");
    }
    else
    {
        // printf("Failed to open valve");
    }
}

void close_bp_valve(void)
{
    if(servo_set_angle(BP_CLOSED_VALVE_ANGLE))
    {
        // printf("Valve was closed successfully");
    }
    else
    {
        // printf("Failed to close valve");
    }
}

void flow_regulator(unsigned int flow_percentage)
{
    unsigned int angle;

    if(flow_percentage>=0 && flow_percentage <= 100)
    {
        angle = map(flow_percentage, 0, 100, 0, 120);
        if(servo_set_angle(angle))
        {
            // printf("Valve in angle position");
        }
        else
        {
            // printf("Valve did not change its position");
        }
    }
    else
    {
        // printf("Invalid flow percentage value");
    }
}