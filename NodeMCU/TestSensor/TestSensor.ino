#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx , valy , valz;
char rd;
int prevVal;
int pin11 = 0 , pin10 = 2 ;
int val1 , val2 ;
int valgy1 = 0 , valgy2 = 0;

#define PCF8591 (0x48)
byte adcvalue0, adcvalue1, adcvalue2, adcvalue3;
float volt;

void setup()
{
  Wire.begin();
  Serial.begin(38400);
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
}
void loop()
{
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);
  Serial.print("axis x = ") ;
  Serial.print(valx) ;
  Serial.print(" axis y = ") ;
  Serial.print(valy) ;
  Serial.print(" axis z = ") ;
  Serial.println(valz) ;
  //////////////////////////////////////////////////
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
  //////////////////////////////////////////////////
  delay(100);
}
