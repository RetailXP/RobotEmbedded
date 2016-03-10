#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H
#include <Arduino.h>

struct StepperMotor
{
	int pulsePin; //pin on on Port C
	int dirPin; // direction pin

	StepperMotor(int pulsePin, int dirPin):
			pulsePin(pulsePin),
			dirPin(dirPin)
	{}
};

void initStepperMotor(StepperMotor motor)
{
	pinMode(motor.pulsePin, OUTPUT);
	pinMode(motor.dirPin, OUTPUT);
	digitalWrite(motor.pulsePin, LOW);
	digitalWrite(motor.dirPin, LOW);
}

void rotateMotor(StepperMotor motor, unsigned int numTicks, int microSecDelay)
{
	for(unsigned int i=1; i<=numTicks; ++i)
	{
		digitalWrite(motor.pulsePin, HIGH);
		delayMicroseconds(microSecDelay);
		digitalWrite(motor.pulsePin, LOW);
		delayMicroseconds(microSecDelay);
	}
}
void changeDirection(int dir, StepperMotor motor)
{
  if (dir == 0)
    digitalWrite(motor.dirPin, LOW);
  else if (dir == 1)
    digitalWrite(motor.dirPin, HIGH);
}

#endif
