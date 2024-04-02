#include <Arduino.h>
#include <WiFi.h>
#include <httpclient.h>
#include <ArduinoJson.h>
#include <time.h>
#include "secrets.h"

// function declerations
long int query_telegram_API();
void press_button(); // function should press and then release


long int update_id = 0; // although initialized to 0, the first query will return ALL values with update ID > 1 hence returning everything
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
  update_id = query_telegram_API();
  if (button_pressed == true) {
    Serial.print("New rint message recieved, pressing button");
    press_button();
    // log accordingly

    Serial.print("Logged successfully");
    button_pressed = false; //reset the parameter
  }
  else {
    // log no button press

    Serial.print("Logged successfully");
  }
  delay(10000);
}


// function definitions
long int query_telegram_API() {
  HTTPClient http;
  String url = "https://api.telegram.org/bot" + BotToken + "/getUpdates?offset=" + (update_id + 1);
  http.begin(url);
  int http_code = http.GET();

  if (http_code == 200) {
    String payload = http.getString();
    Serial.print("payload start < " + payload + " > payload end");
    DynamicJsonDocument doc(1024);
    DeserializationError serialization_error = deserializeJson(doc, payload);
    http.end(); //http connection no longer needed
    // check for parsing errors
      if (serialization_error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(serialization_error.c_str());
        http.end();
        return update_id;
        //log failure
      }

    JsonArray API_updates = doc["result"];

    JsonObject latestUpdate;
    for (JsonObject update : API_updates) {
      latestUpdate = update; // This will keep overwriting with the latest update
    }

    if (!latestUpdate.isNull() && latestUpdate.containsKey("message")) {
      String text = latestUpdate["message"]["text"].as<String>();
      String UNIX_update_time= latestUpdate["message"]["date"].as<String>();
      update_id = latestUpdate["update_id"].as<long>(); // Update the update_id to the latest
      Serial.println(text);
      if (text == "On_PC") {
        press_button();
        button_pressed = true;
        Serial.print("button pressed as of " + UNIX_update_time + "UNIX Time");
        return update_id;
      }
    }

  }
  else {
    Serial.print("could'nt connect to telegram API, return value !!= 200");
    http.end();
    return update_id;
    // log accordingly
  }
}

void press_button() {
  Serial.print("Press button has been called");
}