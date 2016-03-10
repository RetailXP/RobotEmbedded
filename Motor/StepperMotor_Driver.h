#ifndef __STEPPER_MOTOR_DRIVER_H
#define __STEPPER_MOTOR_DRIVER_H

#include "StepperMotor.h"

StepperMotor motor(8, 9);

void setup()
{
    initStepperMotor(motor);
}

void loop()
{
    rotateMotor(motor, 100, HIGH, 100);

    delay(3000);
}

#endif
