void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);
}

void loop()
{
    String msg = "Message From Arduino!!!\n";
    Serial.println(msg);
    delay(1000);
}
