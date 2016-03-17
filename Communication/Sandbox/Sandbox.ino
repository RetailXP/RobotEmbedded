#include <stdint.h>

void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);

    pinMode(13, OUTPUT);
}

int onOff = 1;

void blink()
{
  if(onOff)
  {
    digitalWrite(13, HIGH);
    onOff = 0;
  }
  else
  {
    digitalWrite(13, LOW);
    onOff = 1;
  }
  delay(1000);
}

void loop()
{ 
    blink();
    blink();
    while(!Serial.available());
    blink();
    blink();
    String msg = Serial.readString();
    delay(1000);
    Serial.println(msg);
    Serial.flush();
}

