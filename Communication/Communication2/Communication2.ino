void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(3000);
}

void loop()
{
    Serial.println("Message1");
//    Serial.flush();
//    delay(1000);

//    while(Serial.available())
//    {
        String msg = Serial.readString();
        Serial.println(msg);
//        Serial.flush();
//    }

//    delay(1000);
}
