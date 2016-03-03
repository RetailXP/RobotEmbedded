void setup(){
  Serial.begin(9600);
}

void loop(){
  if (Serial.available())  {
      Serial.println(Serial.read());
  }
  delay(500);
}

void blink(int numberOfTimes){
  for (int i = 0; i < numberOfTimes; i++)  {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
