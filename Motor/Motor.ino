#include "StepperMotor.h"

StepperMotor motor(8, 9);

void setup()
{
    initStepperMotor(motor);
}

void loop()
{
    rotateMotor(motor, 1600, HIGH, 100);

    delay(3000);
}
