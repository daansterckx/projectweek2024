#include <WiFi.h>
#include <HTTPClient.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <ESPAsyncWebServer.h>

HardwareSerial ss(2);
TinyGPSPlus gps;
AsyncWebServer server(80);

const char* ssid = "ESP";
const char* password = "12345678";
const char* serverUrl = "http://192.168.137.187:5000";
const int buttonPin = 23;
const int buzzerPin = 22;

void setup() {
  Serial.begin(9600);
  ss.begin(9600, SERIAL_8N1, 19, 18);
  pinMode(buttonPin, INPUT_PULLUP); // Activeer interne pull-up weerstand voor de knop
  pinMode(buzzerPin, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin("ESP", "12345678");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    delay(1000);
  }
  Serial.print("WebUI IP Address: ");
  Serial.println(WiFi.localIP());

  // Define CORS headers for all responses
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type, Origin, Accept");

  // Handle buzzer activation request
  server.on("/buzzer", HTTP_POST, [](AsyncWebServerRequest *request){
    // Activate the buzzer
    digitalWrite(buzzerPin, HIGH);
    delay(2000); // Buzz for 1 second
    digitalWrite(buzzerPin, LOW); // Turn off the buzzer
    request->send(200, "text/plain", "Buzzer activated successfully!");
  });

  server.begin();
}

void sendLocationToPi(double latitude, double longitude) {
  // Create a JSON document
  DynamicJsonDocument doc(200);
  int buttonState = digitalRead(buttonPin);
  doc["latitude"] = latitude;
  doc["longitude"] = longitude;
  doc["Alarm"] = buttonState;

  // Serialize the JSON document
  String json;
  serializeJson(doc, json);

  // Stuur de JSON-gegevens naar de Pi
  HTTPClient http;
  http.begin(serverUrl);
  http.addHeader("Content-Type", "application/json");
  http.POST(json);
  http.end();
}

void loop() {
  // Controleer of de knop is ingedrukt
  if (digitalRead(buttonPin) == LOW) {
    Serial.println("Kind in nood!");
    // Voeg eventuele extra acties toe die je wilt uitvoeren wanneer de knop wordt ingedrukt
    delay(500);
  }

  // Controleer of er GPS-gegevens beschikbaar zijn
  while (ss.available() > 0) {
    char c = ss.read();

    if (gps.encode(c)) {
      if (gps.location.isValid()) {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);

        // Stuur de locatie naar de Pi
        sendLocationToPi(gps.location.lat(), gps.location.lng());
      } else {
        Serial.println("Location: Not Available");
      }
      delay(500);
    }
  }
}
