#include <DFPlayer_Mini_Mp3.h>
#include <SoftwareSerial.h>
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mp3Serial(D7, D8); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void setup () {
  //  pinMode(PIN_BUSY, INPUT);
  Serial.begin (38400);
  Serial.println("Setting up software serial");
  mp3Serial.begin (9600);

  Serial.println("Setting up mp3 player");
  mp3_set_serial (mp3Serial);
  // Delay is required before accessing player. From my experience it's ~1 sec
  delay(1000);
  Serial.println(F("DFPlayer Mini online."));

  mp3_set_volume (20);

}

void loop () {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '1') {
//      mp3_play(1);
      mp3_play_physical (1);
    }
    if (inChar == '2') {
      mp3_play_physical (2);
    }
    if (inChar == '3') {
      mp3_play_physical (3);
    }
    if (inChar == '7') {
      mp3_play_physical (7);
    }
    else if (inChar == '0') {
      mp3_stop();
    }
  }

  //  delay (6000);
}
