#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Servo.h>   // <-- Corrected
#include <ArduinoJson.h>  // <-- Include this too

const char* ssid = "SETUP-8180";
const char* password = "folded9102dance";

const char* worldTimeApi = "http://worldclockapi.com/api/json/pst/now"; 
const char* scheduleApi = "http://18.191.242.47:5000/get_schedule";

Servo myServo; // <-- Corrected
unsigned long previousMillis = 0;
const long feedingInterval = 15000; // 60 seconds

String meal1 = "08:00";
String meal2 = "13:00";
String meal3 = "18:00";
void fetchSchedule();
void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  myServo.attach(17); // Attach your servo to GPIO17

  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  
  fetchSchedule(); // Get the schedule once at boot
}

String fetchCurrentTime() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(worldTimeApi);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      
      Serial.println("Raw Payload:");
      Serial.println(payload); // Debug print

      StaticJsonDocument<2048> doc;
      DeserializationError error = deserializeJson(doc, payload);
      
      if (!error) {
        String currentDateTime = doc["currentDateTime"].as<String>(); // <-- Different field
        Serial.print("Parsed currentDateTime: ");
        Serial.println(currentDateTime);

        if (currentDateTime.length() >= 16) {
          return currentDateTime.substring(11, 16); // Get "HH:MM" from "2025-04-28T21:44Z"
        }
      } else {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.print("HTTP GET failed, error: ");
      Serial.println(httpCode);
    }
    http.end();
  }
  return "";
}

void fetchSchedule() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scheduleApi);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        meal1 = doc["meal1"].as<String>();
        meal2 = doc["meal2"].as<String>();
        meal3 = doc["meal3"].as<String>();
        
        Serial.println("Meal Schedule Updated:");
        Serial.println(meal1);
        Serial.println(meal2);
        Serial.println(meal3);
      }
    }
    http.end();
  }
}

void openServo() {
  Serial.println("Opening Servo!");
  myServo.write(90); // Open position
  delay(1000);        // Keep open for 1 second
  myServo.write(0);   // Close back
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= feedingInterval) {
    previousMillis = currentMillis;
    
    String currentTime = fetchCurrentTime();
    Serial.print("Current Time: ");
    Serial.println(currentTime);
    
    if (currentTime == meal1 || currentTime == meal2 || currentTime == meal3) {
      openServo();
    }
  }

  delay(10); // <-- Optional: small pause to avoid CPU overload
}
