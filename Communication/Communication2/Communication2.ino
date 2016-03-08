void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);
}

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
    Serial.println("P<A ACK " + msg);
    serialFlush();
    delay(2*1000);
}
