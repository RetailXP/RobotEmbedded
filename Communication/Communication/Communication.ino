void setup()
{
    Serial.begin(9600);
    Serial.setTimeout(2000);
}

bool a = true;

void loop()
{
	String ackMsg = "";
	
	while(a)
	{
		Serial.println("ackMsg: " + ackMsg);

		ackMsg = Serial.readString();

		if(ackMsg.compareTo("ACK") == 0)
		{
			a = false;
			break;
		}
	}
    delay(1000);
}
