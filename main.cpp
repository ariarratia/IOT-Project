#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Servo.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

const char* worldTimeApi = "http://worldtimeapi.org/api/timezone/America/Los_Angeles"; 
const char* scheduleApi = "http://YOUR_SERVER_IP:5000/get_schedule";

Servo myServo;
unsigned long previousMillis = 0;
const long interval = 60000; // 60 seconds

String meal1 = "08:00";
String meal2 = "13:00";
String meal3 = "18:00";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  myServo.attach(17); // Attach your servo to the correct pin

  Serial.print("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  
  fetchSchedule();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    String currentTime = fetchCurrentTime();
    Serial.print("Current Time: ");
    Serial.println(currentTime);
    
    if (currentTime == meal1 || currentTime == meal2 || currentTime == meal3) {
      openServo();
    }
  }
}

String fetchCurrentTime() {
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(worldTimeApi);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (!error) {
        String datetime = doc["datetime"]; // "2025-04-28T17:28:34.564838-07:00"
        return datetime.substring(11, 16); // Get "HH:MM"
      }
    }
    http.end();
  }
  return "";
}

void fetchSchedule() {
  if ((WiFi.status() == WL_CONNECTED)) {
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
  myServo.write(90); // Open Position (adjust based on your servo)
  delay(2000); // Hold open for 2 seconds
  myServo.write(0);  // Close Position
}
