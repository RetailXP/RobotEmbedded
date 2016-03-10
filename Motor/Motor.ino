#include "StepperMotor.h"

StepperMotor motor(8, 9);

void setup()
{
    initStepperMotor(motor);
}

void loop()
{
    rotateMotor(motor, 1600, 100);

    delay(3000);
}
