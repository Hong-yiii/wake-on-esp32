#include <Arduino.h>
#include <WiFi.h>
#include <httpclient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"


long last_update_time = 0;// have to implement after learning time.h

// put function declarations here:
int myFunction(int, int);

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
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}