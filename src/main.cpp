#include <Arduino.h>
#include <WiFi.h>
#include <httpclient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"

// function declerations
void query_telegram_API(bool button_pressed);
void press_button();


long last_update_time = 0; // have to implement after learning time.h
bool button_pressed = false; // when turning on, set to true, then back to false

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Wifi is not WIFIing");
  }
  // display connected when it has connected
  if( WiFi.status() == WL_CONNECTED) {
    Serial.print("WIFI is WIFIing");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  query_telegram_API();
  if (button_pressed == true) {
    Serial.print("New rint message recieved, pressing button");
    press_button();
    // log accordingly

    Serial.print("Logged successfully");
  }
  else {
    // log no button press

    Serial.print("Logged successfully");
  }
  delay(10000);
}


// function definitions
void query_telegram_API(bool button_pressed) {

}

void press_button() {

}