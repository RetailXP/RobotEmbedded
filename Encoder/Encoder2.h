#ifndef __ENCODER2_H
#define __ENCODER2_H

#include <pins_arduino.h>

// per revolusion, the encoder registers 800 ticks

// VCC: Red, GND: Black, A: Green, B: White
#define encoder2PinA  2
#define encoder2PinB  23

volatile int encoder2Pos = 0;

void encoder2Interr()
{
    /*
    * If pinA and pinB are both high or both low, it is spinning
    * forward. If they're different, it's going backward.
    *
    * For more information on speeding up this process, see
    * [Reference/PortManipulation], specifically the PIND register.
    */

    if(digitalRead(encoder2PinA) == digitalRead(encoder2PinB))
    {
        encoder2Pos++;
    } else
    {
        encoder2Pos--;
    }
}

// to be called within setup
void initEncoder2()
{
    pinMode(encoder2PinA, INPUT); 
    digitalWrite(encoder2PinA, HIGH);       // turn on pullup resistor
    pinMode(encoder2PinB, INPUT); 
    digitalWrite(encoder2PinB, HIGH);       // turn on pullup resistor
    attachInterrupt(digitalPinToInterrupt(encoder2PinA),
                    encoder2Interr,
                    CHANGE);  // encoder pin on interrupt 0 - pin 2
}

#endif
