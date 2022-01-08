#define rain A0
void setup() { 

  Serial.begin(38400); 

}   

void loop() { 

  int sensorValue = analogRead(rain); 

  Serial.println(sensorValue); 

  delay(100);

}
