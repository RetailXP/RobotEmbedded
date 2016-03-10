#include "Encoder1.h"
#include "Encoder2.h"

int tickEncCount1 = 0;
int tickEncCount2 = 0;
  
void setup()
{
    initEncoder1();
    initEncoder2();

    Serial.begin(9600);
}

void loop()
{
    if (tickEncCount1 != encoder1Pos)
    {
        Serial.print("Encoder1: ");
        Serial.println(encoder1Pos, DEC);
        Serial.println();
        tickEncCount1 = encoder1Pos;
    }

    if (tickEncCount2 != encoder2Pos)
    {
        Serial.print("Encoder2: ");
        Serial.println(encoder2Pos, DEC);
        Serial.println();
        tickEncCount2 = encoder2Pos;
    }
}
