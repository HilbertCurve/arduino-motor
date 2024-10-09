#include "conf.h"

servo_t servos[2];
stepper_t steppers[2];

void initMotors() {
    servos[0].pin = 5;
    servos[1].pin = 3;

    steppers[0].pin[0] = 13;
    steppers[0].pin[1] = 12;
    steppers[0].pin[2] = 11;
    steppers[0].pin[3] = 10;
    steppers[0].stepsPerRev = 2052;
    steppers[1].pin[0] = 13;
    steppers[1].pin[1] = 12;
    steppers[1].pin[2] = 11;
    steppers[1].pin[3] = 10;
    steppers[1].stepsPerRev = 2052;
}