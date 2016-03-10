#ifndef __ENCODER_H
#define __ENCODER_H

// #define encoder0PinA  2
// #define encoder0PinB  3

// volatile unsigned int encoder0Pos = 0;

class Encoder
{
public:
    Encoder(int encoderPinA, int encoderPinB, int interruptPin):
            cm_encoderPinA(encoderPinA),
            cm_encoderPinB(encoderPinB),
            cm_interruptPin(interruptPin),
            m_encoderPos(0)
    {}

    void getEncoderPos();
    void encoderInterrupt();
    void init();


private:
    const int cm_encoderPinA;
    const int cm_encoderPinB;
    const int cm_interruptPin;

    volatile unsigned int m_encoderPos;
};

void Encoder::getEncoderPos()
{
    return m_encoderPos;
}

void Encoder::encoderInterrupt()
{
    /*
    * If pinA and pinB are both high or both low, it is spinning
    * forward. If they're different, it's going backward.
    *
    * For more information on speeding up this process, see
    * [Reference/PortManipulation], specifically the PIND register.
    */

    if (digitalRead(cm_encoderPinA) == digitalRead(cm_encoderPinB))
    {
        m_encoderPos++;
    } else
    {
        m_encoderPos--;
    }
}

// to be called within setup
void Encoder::init()
{
	pinMode(cm_encoderPinA, INPUT); 
	digitalWrite(cm_encoderPinA, HIGH);       // turn on pullup resistor
	pinMode(cm_encoderPinB, INPUT); 
	digitalWrite(cm_encoderPinB, HIGH);       // turn on pullup resistor
	attachInterrupt(cm_interruptPin, encoderInterrupt, CHANGE);  // encoder pin on interrupt 0 - pin 2
}

#endif