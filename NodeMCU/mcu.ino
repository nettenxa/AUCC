#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"

#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "Net2.4G" 
#define WIFI_PASSWORD "054015698"

#define FIREBASE_HOST "elcare-mcu-default-rtdb.firebaseio.com" 
#define FIREBASE_KEY "1BA25vurmM1TEVC5Ug1jxOLE4MmHDnVRasGev1yn"
FirebaseData firebaseData;
bool state = false;

MPU6050 mpu;
// Gyro
int16_t ax, ay, az;
int16_t gx, gy, gz;
int valx , valy , valz;
char rd;
int prevVal;
int pin11 = 5 , pin10 = 4 ;
int val1 , val2 ;
int valgy1 = 0 , valgy2 = 0;
//

// Ultrasonic 
const int pingPin = 14; //D1
int inPin = 12; //D2
//

// Laser input
#define pin A0
//

int buzz = 16; //D0
int count=0;
// Click to Alert
int buttonAlt = 2;
int buttonState = 0;
int alert=0;
int returnAlert=0;
int returnTime=0;
//

void setup() {
  Serial.begin (38400);
  pinMode(buttonAlt, INPUT);
  pinMode(pin, INPUT);  
  pinMode(buzz, OUTPUT);
  // Wifi + Firebase Connect
  Wire.begin();
  Serial.println("Initialize MPU");
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "Connected" : "Connection failed");
  connectWifi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
  //
}
 
void loop() {
  buttonState = digitalRead(buttonAlt);  
  if (buttonState == 0) {
    Serial.println("Click");
    alert+=1;
    delay(200);
    if(alert > 4) { // start = 0 end = 5 : alert = 5
      Serial.println("Net");    
      digitalWrite(buzz, LOW);
      returnAlert = 1;
    }
  }
  else {
//    Serial.println(alert);
    digitalWrite(buzz, HIGH);    
    alert=0;
  }

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
  Serial.print("Ultrasonic: ");
  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();

/////////////////// Lazer
  uint16_t value = analogRead (pin);
  double distance = get_IR (value);
  distance = distance*2;
  if (distance > 150) {
    distance = 150;
  }
  else if (distance < 30) {
    distance = cm;
  }
  //////////////////////////////

  else if (cm > 140 && distance > 140) {
    digitalWrite(buzz, HIGH);
  }  
  else if (cm > 120 && distance > 120) {
    digitalWrite(buzz, HIGH);
    delay(500);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.print("120");
  }  
  else if (cm > 80 && distance > 80) {
    digitalWrite(buzz, HIGH);
    delay(300);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.print("80");
  } 
  else if (cm > 50 && distance > 50) {
    digitalWrite(buzz, HIGH);
    delay(250);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.print("50");
  } 
  else if (cm > 20 && distance > 20) {
    digitalWrite(buzz, HIGH);
    delay(50);
    digitalWrite(buzz, LOW);
    delay(100);
    Serial.print("20");
  } 
  if (cm < 20 ) {
    digitalWrite(buzz, LOW);
  }

//////////////////////////////
  Serial.print ("Laser: ");
  Serial.print (distance);
  Serial.println (" cm ");
  Serial.println ();

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  valx = map(ax, -17000, 17000, 0, 179);
  valy = map(ay, -17000, 17000, 0, 179);
  valz = map(az, -17000, 17000, 0, 179);

    FirebaseJson data;
    if(Firebase.setJSON(firebaseData, "/wio-link-01/", data)) {
      Firebase.setInt(firebaseData, "valx", valx);
      Firebase.setInt(firebaseData, "valy", valy);
      Firebase.setInt(firebaseData, "valz", valz);
      if(returnAlert == 1) {
         Firebase.setInt(firebaseData, "Alert", returnAlert);
         returnAlert=0;
      }
      else {
        Firebase.setInt(firebaseData, "Alert", returnAlert);
      }
    } else {
        Serial.println("Error : " + firebaseData.errorReason());
    }
      
  Serial.print("axis x = ") ;
  Serial.print(valx) ;
  Serial.print(" axis y = ") ;
  Serial.print(valy) ;
  Serial.print(" axis z = ") ;
  Serial.println(valz) ;
//  if(valx < 50 || valy < 50 || valz < 50) {
//    returnTime+=1;
//    if(returnTime > 10) {
//      digitalWrite(buzz, LOW);
//    }
//    delay(1000);
//  }
}
 
long microsecondsToCentimeters(long microseconds) {
return microseconds / 29 / 2;
}

//return distance (cm)
double get_IR(uint16_t value) {
  if (value < 16)  value = 16;
  return 9462.0 / (value - 16.92);
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
