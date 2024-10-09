#include <Arduino.h>

typedef struct {
    size_t pin;
} servo_t;

typedef struct {
    size_t pin[4];
    size_t stepsPerRev;
} stepper_t;

extern servo_t servos[2];
extern stepper_t steppers[2];