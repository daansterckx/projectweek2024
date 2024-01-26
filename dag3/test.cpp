#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>

// The serial connection to the GPS module
HardwareSerial ss(2);

TinyGPSPlus gps;

// WiFi credentials
const char* ssid = "DAANSLAPTOP1";
const char* password = "12345678";

// Server URL
const char* serverUrl = "http://192.168.137.36:5000";

void setup() {
  Serial.begin(115200);
  ss.begin(9600, SERIAL_8N1, 16, 17); // RX=16, TX=17

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      // Create a JSON document
      StaticJsonDocument<200> doc;
      doc["latitude"] = gps.location.lat();
      doc["longitude"] = gps.location.lng();

      // Serialize the JSON document
      String json;
      serializeJson(doc, json);

      // Send the JSON data to the server
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(json);
        http.end();
      }
    }
  }
}