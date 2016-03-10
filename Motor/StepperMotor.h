#ifndef __STEPPER_MOTOR_H
#define __STEPPER_MOTOR_H

struct StepperMotor
{
	int pulsePin;
	int dirPin;		// direction pin

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

// the dir parameter should either be HIGH or LOW
void rotateMotor(StepperMotor motor, int numTicks, int dir, int microSecDelay)
{
  digitalWrite(motor.dirPin, dir);
  
	for(int i=1; i<=numTicks; ++i)
	{
		digitalWrite(motor.pulsePin, HIGH);
		delayMicroseconds(microSecDelay);
		digitalWrite(motor.pulsePin, LOW);
		delayMicroseconds(microSecDelay);
	}
}

#endif
