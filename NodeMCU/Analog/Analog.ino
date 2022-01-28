#include "Wire.h"
#define PCF8591 (0x48)
byte adcvalue0, adcvalue1, adcvalue2, adcvalue3;
float volt;
#define LED_PIN 13

void setup() {
  Wire.begin();
  Serial.begin(38400);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  Wire.beginTransmission(PCF8591);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.requestFrom(PCF8591, 5);
  adcvalue0 = Wire.read();
  adcvalue0 = Wire.read();
  adcvalue1 = Wire.read();
  adcvalue2 = Wire.read();
  adcvalue3 = Wire.read();
  Serial.print("AIN0=");
  volt = (adcvalue0 * 5.0) / 255;
  Serial.println(volt);
  Serial.print("AIN1=");
  volt = (adcvalue1 * 5.0) / 255;
  Serial.println(volt);
  Serial.print("AIN2=");
  volt = (adcvalue2 * 5.0) / 255;
  Serial.println(volt);
  Serial.print("AIN3=");
  volt = (adcvalue3 * 5.0) / 255;
  Serial.println(volt);
  Serial.println("");
  Wire.beginTransmission(PCF8591);
  Wire.write(0x40);
  Wire.write(adcvalue0);
  Wire.endTransmission();
  delay(500);
}
