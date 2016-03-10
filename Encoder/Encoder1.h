#ifndef __ENCODER1_H
#define __ENCODER1_H

#include <pins_arduino.h>

/**
*   The reason for having separate header files for each encoder is to be able to attach
*   interrupts. The 'attachInterrupt' function does not accept class function as a function pointer
*   and parameters cannot be passed to the ISR. Thus, this encapsulates all pin numbers
*   and encoderCount for each encoder, despite some duplication of codes.
**/

// per revolusion, the encoder registers 800 ticks

// VCC: Red, GND: Black, A: Green, B: White
#define encoder1PinA  3
#define encoder1PinB  22

volatile int encoder1Pos = 0;

void encoder1Interr()
{
    /*
    * If pinA and pinB are both high or both low, it is spinning
    * forward. If they're different, it's going backward.
    *
    * For more information on speeding up this process, see
    * [Reference/PortManipulation], specifically the PIND register.
    */

    if(digitalRead(encoder1PinA) == digitalRead(encoder1PinB))
    {
        encoder1Pos++;
    } else
    {
        encoder1Pos--;
    }
}

// to be called within setup
void initEncoder1()
{
    pinMode(encoder1PinA, INPUT); 
    digitalWrite(encoder1PinA, HIGH);       // turn on pullup resistor
    pinMode(encoder1PinB, INPUT); 
    digitalWrite(encoder1PinB, HIGH);       // turn on pullup resistor
    attachInterrupt(digitalPinToInterrupt(encoder1PinA),
                    encoder1Interr,
                    CHANGE);  // encoder pin on interrupt 0 - pin 2
}

#endif
