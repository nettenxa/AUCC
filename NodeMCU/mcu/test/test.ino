// 
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "Net2.4G" 
#define WIFI_PASSWORD "054015698"

#define FIREBASE_HOST "elcare-mcu-default-rtdb.firebaseio.com" 
#define FIREBASE_KEY "1BA25vurmM1TEVC5Ug1jxOLE4MmHDnVRasGev1yn"

FirebaseData firebaseData;
bool state = false;

void setup() {
    connectWifi();
    Firebase.begin(FIREBASE_HOST, FIREBASE_KEY);
}

void loop() {
    delay(1000);
    state = !state;
    if(Firebase.setBool(firebaseData, "/wio-link-01/state", state)) {
        Serial.println("Added"); 
    } else {
        Serial.println("Error : " + firebaseData.errorReason());
    }
}

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
