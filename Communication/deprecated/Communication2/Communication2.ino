void setup()
{
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

    Serial.begin(9600);
    Serial.setTimeout(2000);
}

//void blink()
//{
//  if(onOff)
//  {
//    digitalWrite(13, HIGH);
//    onOff = 0;
//  }
//  else
//  {
//    digitalWrite(13, LOW);
//    onOff = 1;
//  }
//  delay(1000);
//}

void serialFlush()
{
    while(Serial.available() != 0)
    {
      char t = Serial.read();
    }
}

void loop()
{
    while(!Serial.available());

    String msg = Serial.readString();

    if(msg.indexOf("P>A") != -1)
    {
        digitalWrite(13, HIGH);
        delay(1000);

        digitalWrite(13, LOW);
        delay(1000);
    }

    Serial.println("P<A ACK " + msg);
}
