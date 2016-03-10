//direction 0 = counter clockwise when looking at motor front
//A+ = Red/Blue
//A- = Yellow/Black
//B+ = Orange/Green
//B- = Brown/White
//direction 1 = clockwise

//direction 0 = clockwise when looking at motor front
//A+ = Yellow/Black
//A- = Red/Blue
//B+ = Orange/Green
//B- = Brown/White

#include "StepperMotor.h"

StepperMotor motor(9, 8);
unsigned int a = 0;
void setup()
{
    initStepperMotor(motor);
    Serial.begin(9600);
}

void loop()
{
  if (Serial.available())
  {
    a = Serial.parseInt();
    if (a == 0 || a == 1)
      changeDirection(a, motor);
    else if (a<50001)
      rotateMotor(motor, a, 20);
    Serial.println(a);
  }
}
