// Import Gyro Lib
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
//
// Import DFPlayer Lib
#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"
//

// GYRO //
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx , valy , valz;
// END-GYRO //

// DFPlayer //
SoftwareSerial mp3Serial(D7, D8); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
// END-DFPlayer //

// Infrared //
#define pin A0
// END-Infrared //

// Ultrasonic
const int pingPin = 14; //D1
int inPin = 12; //D2
//

// Click to Alert //
int buttonAlt = D0;
int buttonState = 0;
int alert = 0;
int alertTEST = 0;
int returnAlert = 0;
int returnTime = 0;
// END //

// Wifi + Firebase //
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "Net_2.4G"
#define WIFI_PASSWORD "0861962930"
//#define WIFI_SSID "NettenXA"
//#define WIFI_PASSWORD "net123456789"
//#define WIFI_SSID "Net1"
//#define WIFI_PASSWORD "123456789o"


#define FIREBASE_HOST "elcare-mcu-default-rtdb.firebaseio.com"
#define FIREBASE_KEY "1BA25vurmM1TEVC5Ug1jxOLE4MmHDnVRasGev1yn"
FirebaseData firebaseData;
// END //

// Analog //
#define PCF8591 (0x48)
byte adcvalue0, adcvalue1, adcvalue2, adcvalue3;
float volt;
//END-Analog //

void setup()
{
  Wire.begin();
  Serial.begin(38400);
  // Check GYRO
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  // END //

  // DFPlayer //
  Serial.println("Setting up software serial");
  mp3Serial.begin (9600);

  Serial.println("Setting up mp3 player");
  mp3_set_serial (mp3Serial);
  // Delay is required before accessing player. From my experience it's ~1 sec
  //  delay(1000);
  Serial.println(F("DFPlayer Mini online."));

//  mp3_set_volume (0); // Volump
  mp3_set_volume (3); // Volump
//  mp3_set_volume (20); // Volump
  // END-DFPlayer //

  // Infrared //
  pinMode(pin, INPUT);
  // END-Infrared //

  // Wifi + Firebase Connect
  Wire.begin();
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  connectWifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  mp3_play_physical (4);
  delay(5000);
  //
}
void loop()
{
//  mp3_set_volume (0); // Volump
//  mp3_set_volume (6); // Volump
//  mp3_set_volume (15); // Volump
//  mp3_set_volume (20); // Volump

  buttonState = digitalRead(buttonAlt);
  if (buttonState == HIGH) {
    Serial.println("Click");
    alert += 1;
    alertTEST += 1;
    //    delay(200);
    if (alertTEST > 4) { // start = 0 end = 5 : alert = 5
      Serial.println("แหล่งอำนวยความสะดวก");
      mp3_play_physical (1);
      alertTEST = 0;
      delay(5000);
      //      mp3_play_physical (5);
      //      delay(2000);
    }
    if (alert > 5) { // start = 0 end = 5 : alert = 5
      Serial.println("Net");
//      mp3_play_physical (7);
      returnAlert = 1;
    }
  }
  else {
    Serial.println("Don't Click");
    Serial.println(alert);
    //    mp3_play_physical (7);
    alert = 0;
  }

  // Gyro-Variable //
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);
  Serial.print("x = ") ;
  Serial.print(valx) ;
  Serial.print(" y = ") ;
  Serial.print(valy) ;
  Serial.print(" z = ") ;
  Serial.println(valz) ;
  // Gyro-Var //

  // Audio //
  //  while (Serial.available()) {
  //    char inChar = (char)Serial.read();
  //    if (inChar == '1') {
  //      mp3_play_physical (1);
  //    }
  //    if (inChar == '2') {
  //      mp3_play_physical (2);
  //    }
  //      if (inChar == '3') {
  //        mp3_play_physical (3);
  //      }
  //    if (inChar == '7') {
  //      mp3_play_physical (7);
  //    }
  //    else if (inChar == '0') {
  //      mp3_stop();
  //    }
  //  }
  // END-Audio //

  // Ultrasonic //

  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(inPin, INPUT);
  duration = pulseIn(inPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  // END-Ultrasonic //

  // Infrared //
  uint16_t value = analogRead (pin);
  double distance = get_IR (value);
  distance = distance * 2;
  if (distance > 150) {
    distance = 150;
  }
//  else if (distance < 30) {
//    distance = cm;
//  }
  // END-Infrared //

  // Print Check //
  Serial.print("Ultrasonic: ");
  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();
  Serial.print ("Laser: ");
  Serial.print (distance);
  Serial.println (" cm ");
  Serial.println ();
  // END Print Check //
  //////////////////////////////
//  if (cm > 121 && distance > 121) {
//    mp3_play_physical (0);
//  }
//  else if (cm > 120 && distance > 120) {
//    mp3_play_physical (7);
//    delay(1000);
//    //    digitalWrite(buzz, LOW);
//    //    delay(100);
//    Serial.print("120");
//    Serial.print("\n");
//  }
//  else if (cm > 80 && distance > 80) {
//    mp3_play_physical (7);
//    delay(500);
//    //    digitalWrite(buzz, LOW);
//    //    delay(100);
//    Serial.print("80");
//    Serial.print("\n");
//  }
//  else if (cm > 50 && distance > 50) {
//    mp3_play_physical (7);
//    delay(300);
//    //    digitalWrite(buzz, LOW);
//    //    delay(100);
//    Serial.print("50");
//    Serial.print("\n");
//  }
//  else if (cm > 20 && distance > 20) {
//    mp3_play_physical (7);
//    delay(10);
//    //    digitalWrite(buzz, LOW);
//    //    delay(100);
//    Serial.print("20");
//    Serial.print("\n");
//  }
//  if (cm < 20 ) {
//    //    digitalWrite(buzz, LOW);
//    mp3_play_physical (7);
//  }
if (distance > 121) {
    mp3_play_physical (0);
  }
  else if (distance > 120) {
    mp3_play_physical (7);
    delay(1000);
    //    digitalWrite(buzz, LOW);
    //    delay(100);
    Serial.print("120");
    Serial.print("\n");
  }
  else if (distance > 80) {
    mp3_play_physical (7);
    delay(500);
    //    digitalWrite(buzz, LOW);
    //    delay(100);
    Serial.print("80");
    Serial.print("\n");
  }
  else if (distance > 50) {
    mp3_play_physical (7);
    delay(300);
    //    digitalWrite(buzz, LOW);
    //    delay(100);
    Serial.print("50");
    Serial.print("\n");
  }
  else if (distance > 20) {
    mp3_play_physical (7);
    delay(10);
    //    digitalWrite(buzz, LOW);
    //    delay(100);
    Serial.print("20");
    Serial.print("\n");
  }
  if (cm < 20 ) {
    //    digitalWrite(buzz, LOW);
    mp3_play_physical (7);
  }
  //////////////////////////////



  // Rain-Drop //
  Wire.beginTransmission(PCF8591);
  Wire.write(0x04);
  Wire.endTransmission();
  Wire.requestFrom(PCF8591, 5);
  adcvalue0 = Wire.read();
  Serial.print("AIN0=");
  volt = (adcvalue0 * 5.0) / 255;
  Serial.println(volt);

  int rainCount = 0;
  if (volt > 0.1) {
    Serial.println("Rain: ");
    Serial.println(rainCount);
    if (rainCount == 0) {
      mp3_play_physical (2);
      rainCount = 1;

      if (rainCount == 1) {
        delay(5000);
        rainCount = 0;
        volt = 0.00;
      }
    }
    //    delay(9000);
  }
  volt = 0.00;
  // END-Rain //
  // Drop //
  if (valx > 100) {
    if (valy > 90) {
      if (valz > 0) {
        mp3_play_physical (7);
        delay(1000);
      }
    }
  }
  // END-DROP //

  // FireBase //
//  bool state = true;
//  if (Firebase.setInt(firebaseData, "/MCU/valx", valx)) {
//    Serial.println("Added");
//  }
//  if (Firebase.setInt(firebaseData, "/MCU/valy", valy)) {
//    Serial.println("Added");
//  }
//  if (Firebase.setInt(firebaseData, "/MCU/valz", valz)) {
//    Serial.println("Added");
//  }
//  if (returnAlert == 1) {
//    if (Firebase.setInt(firebaseData, "/MCU/Alert", returnAlert)) {
//      Serial.println("Added");
//    }
//    //    Firebase.setInt(firebaseData, "Alert", returnAlert);
//    returnAlert = 0;
//  }
//  else {
//    //    Firebase.setInt(firebaseData, "Alert", returnAlert);
//    if (Firebase.setInt(firebaseData, "/MCU/Alert", returnAlert)) {
//      Serial.println("Added");
//    }
//  }
  // END-FireBase //

  delay(100);
}

//return distance (cm)For Infrared Sensor
double get_IR(uint16_t value) {
  if (value < 16)  value = 16;
  return 9462.0 / (value - 16.92);
}

//return distance (cm)For Infrared Sensor
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

// Connect Wifi
void connectWifi() {
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}
