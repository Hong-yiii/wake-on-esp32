#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"

// function declerations
long int query_telegram_API();
void press_button(); // function should press and then release


const int Pmos_gate = 13;
long int update_id = 0; // although initialized to 0, the first query will return ALL values with update ID > 1 hence returning everything
bool button_pressed = false; // when turning on, set to true, then back to false

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(Pmos_gate, OUTPUT);
  digitalWrite(Pmos_gate, HIGH);  
  WiFi.begin(ssid, password);
  while( WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi is not WIFIing");
    delay(10000);
  }
  // display connected when it has connected
  if( WiFi.status() == WL_CONNECTED) {
    Serial.println("WIFI is WIFIing");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  update_id = query_telegram_API();
  if (button_pressed == true) {
    Serial.println("New rint message recieved, pressing button");
    press_button();
    // log accordingly

    Serial.println("Logged successfully");
    button_pressed = false; //reset the parameter
  }
  else {
    // log no button press

    Serial.println("Logged successfully");
  }
  delay(10000);
}


// function definitions
long int query_telegram_API() {
  HTTPClient http;
  Serial.println(update_id);
  String url = "https://api.telegram.org/bot" + BotToken + "/getUpdates?offset=" + (update_id + 1);
  http.begin(url);
  int http_code = http.GET();

  if (http_code == 200) {
    String payload = http.getString();
    Serial.println("payload start < " + payload + " > payload end");

    StaticJsonDocument<1024> doc;
    DeserializationError serialization_error = deserializeJson(doc, payload);

    http.end();
    // check for parsing errors
      if (serialization_error) {
        Serial.println("deserializeJson() failed: ");
        Serial.println(serialization_error.c_str());
        return update_id;
        //log failure
      }

    JsonArray API_updates = doc["result"];

    for (JsonObject update : API_updates) {
      long current_update_id = update["update_id"].as<long>();
      if (current_update_id > update_id) {
        update_id = current_update_id; // Update the update_id to the highest one received
      }

    if (update.containsKey("message")) {
        String text = update["message"]["text"].as<String>();
        long UNIX_update_time = update["message"]["date"].as<long>();
        Serial.println(text);
        if (text == "On_PC") {
          press_button();
          button_pressed = true;
          Serial.println("button pressed as of " + String(UNIX_update_time) + " UNIX Time");
          // Don't return yet, we need to process all updates
        }
      }
    }
    return update_id; // After processing all updates, return the highest update_id encountered
  }
  else {
    Serial.println("could'nt connect to telegram API, return value !!= 200");
    http.end();
    return update_id;
    // log accordingly
  }
}

void press_button() {
  Serial.println("Press button has been called");
  // Set the GPIO pin LOW to turn on the PMOS (simulate button press)
  digitalWrite(Pmos_gate, LOW);
  
  // Wait for 1 second
  delay(1000); // 1000 milliseconds = 1 second
  
  // Set the GPIO pin back to HIGH to turn off the PMOS (end button press simulation)
  digitalWrite(Pmos_gate, HIGH);
}