#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Servo.h>
#include <ArduinoJson.h>
#include "HX711.h"

// WiFi credentials
const char* ssid = "SETUP-8180";
const char* password = "folded9102dance";

// API endpoints
const char* worldTimeApi = "http://worldclockapi.com/api/json/pst/now"; 
const char* scheduleApi = "http://18.191.242.47:5000/get_schedule";

// Servo setup
Servo myServo;

// Load cell setup
HX711 scale;
uint8_t dataPin = 13;
uint8_t clockPin = 12;

float w1, w2;

// Feeding schedule
String meal1 = "08:00";
String meal2 = "13:00";
String meal3 = "18:00";

unsigned long previousMillis = 0;
const long feedingInterval = 15000; // Adjust as needed
void fetchSchedule();
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  myServo.attach(17);

  scale.begin(dataPin, clockPin);
  scale.set_offset(558795.5);
  scale.set_scale(-4471.4561765);
  scale.tare();

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  fetchSchedule();
}

void fetchSchedule() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scheduleApi);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<1024> doc;
      if (deserializeJson(doc, payload) == DeserializationError::Ok) {
        meal1 = doc["meal1"].as<String>();
        meal2 = doc["meal2"].as<String>();
        meal3 = doc["meal3"].as<String>();
      }
    }
    http.end();
  }
}

String fetchCurrentTime() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(worldTimeApi);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<2048> doc;
      if (deserializeJson(doc, payload) == DeserializationError::Ok) {
        String currentDateTime = doc["currentDateTime"].as<String>();
        if (currentDateTime.length() >= 16) {
          return currentDateTime.substring(11, 16);
        }
      }
    }
    http.end();
  }
  return "";
}

bool isPlateEmpty() {
  w1 = scale.get_units(10);
  delay(100);
  w2 = scale.get_units();
  while (abs(w1 - w2) > 10) {
    w1 = w2;
    w2 = scale.get_units();
    delay(100);
  }
  Serial.print("Current plate weight: ");
  Serial.println(w1);
  return w1 < 6; // weight in grams
}

void openServo() {
  Serial.println("Opening Servo!");
  myServo.write(90);
  delay(1000);
  myServo.write(0);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= feedingInterval) {
    previousMillis = currentMillis;
    String currentTime = fetchCurrentTime();
    Serial.print("Current Time: ");
    Serial.println(currentTime);

    if (currentTime == meal1 || currentTime == meal2 || currentTime == meal3) {
      if (isPlateEmpty()) {
        openServo();
      } else {
        Serial.println("Food already in bowl. Skipping feed.");
      }
    }
  }

  delay(10);
}
