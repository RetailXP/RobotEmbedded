void setup(){
  Serial.begin(9600);
}

void loop(){
  Serial.println("Hello, World!");
  delay(1000);
  byte message = Serial.read();
  Serial.println()
}
