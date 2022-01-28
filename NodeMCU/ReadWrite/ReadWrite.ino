#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "Net_2.4G" 
#define WIFI_PASSWORD "0861962930"

#define FIREBASE_HOST_Mobile "elcare-map-service-default-rtdb.firebaseio.com/Location"
#define FIREBASE_KEY_Mobile "6CyDhOApa9oR1FzNWIKW114JMC5ha2IbLOSFEGBy"

FirebaseData firebaseData_Mobile;
bool state = false;

void setup() {
    connectWifi();
    Firebase.begin(FIREBASE_HOST_Mobile, FIREBASE_KEY_Mobile);
    // Do something
}

void loop() {
  delay(1000);
//  Serial.println("NetteN");
  if (Firebase.getJSON(firebaseData_Mobile, "/Location")) {
    FirebaseJson *json = firebaseData_Mobile.jsonObjectPtr();
    size_t len = json->iteratorBegin();
    String key, value;
    int type = 0;
    for (size_t index = 0; index < len; index++)
    {
        yield();
        json->iteratorGet(index, type, key, value);
        if (type == FirebaseJson::JSON_OBJECT && key.length() > 1) {
            
//            Serial.println(key);
            if (key == "latitude"){
//              Serial.println(key);
//              Serial.println(" : " + value);
                Serial.println(key + " : " + value);
            }
            if (key == "longitude"){
//              Serial.println(key);
//              Serial.println(" : " + value);
               Serial.println(key + " : " + value);
            }

        }
    }
    json->iteratorEnd();
    json->clear();
    // Do something
  } else {
      Serial.println("Error : " + firebaseData_Mobile.errorReason());
  }

  
}

void connectWifi() {
    Serial.begin(38400);
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
