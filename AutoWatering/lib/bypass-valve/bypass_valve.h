#ifndef BYPASS_VALVE_H
#define BYPASS_VALVE_H

#include "servo_pwm.h"

#define BP_CLOSED_VALVE_ANGLE 0
#define BP_FULLY_OPEN_VALVE_ANGLE 120
#define BP_HALF_OPEN_VALVE_ANGLE 60

#define FLOW_PERCENTAGE_2_ANGLE (1.2f)


void open_bp_valve(void);

void close_bp_valve(void);


#endif /* BYPASS_VALVE_H */