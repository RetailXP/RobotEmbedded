void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);
}

int num = 0;

void loop()
{
    String msg = "Message From Arduino!!!";
    Serial.println(num);
    num++;
    // delay(1000);
}
