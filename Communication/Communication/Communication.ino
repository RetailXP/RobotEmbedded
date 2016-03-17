#include <stdint.h>
#include "codeMsg.h"

#define RET_INV 0x01
#define DEP_INV 0x02
#define HOME	0x03
#define MOVE	0x04

#define MAX_BUFFER_LEN 32
/*
- interrupts for limit switches are only used for zeroing

1) wait for message
2) process message
  a) homing
  b) pick and place
    i) Coordinate to move to pick up
    ii) coordinate to drop off
    iii) shoe box above or not?
      -y direction of shoe box above
    iv) exceptions (top box - use top robot)

*/


//------------------------ SWITCH DEFINE ------------------------//



//------------------------MESSAGE PASSING------------------------//
//receive variables
uint8_t dataBuf[20];
uint8_t encodeChar;
uint8_t msgLen=0;
uint8_t dataBufLen;
uint8_t bytesParsed;

DECODE_STATE_t decodeState = DECODE_STATE_INIT;

//send variables
uint8_t txBuffer[32];
uint8_t bytesToSend;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
   if (Serial.available())
  {
    encodeChar = Serial.read();
    decodeState = CodeMsg_DecodeEscapedMsg(decodeState, &msgLen, &encodeChar, 1, &bytesParsed, dataBuf, &dataBufLen);
    
    if (decodeState == DECODE_STATE_COMPLETE)
    {
        if (dataBuf[0] == RET_INV)
        {
            bytesToSend = CodeMsg_CreateEscapedTxMsg(dataBuf, 3, MAX_BUFFER_LEN, txBuffer); //create a 3 Byte long return message
            Serial.write(txBuffer, bytesToSend);
        }
        else if (dataBuf[0] == DEP_INV)
        {

        }
        else if (dataBuf[0] == HOME)
        {
          
        }
        else if (dataBuf[0] == MOVE)
        {
          
        }
        else
        {
          //error
        }
        
        decodeState = DECODE_STATE_INIT;
    }
  
    
  }
}
