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

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    String html = "<html><head>";
    html += "<meta http-equiv=\"refresh\" content=\"5\">"; // Vernieuwen elke 5 seconden
    html += "</head><body>";
    html += "<h1>ESP32 GPS Data</h1>";
    
    if (gps.location.isValid()) {
      html += "<p>Latitude: " + String(gps.location.lat(), 6) + "</p>";
      html += "<p>Longitude: " + String(gps.location.lng(), 6) + "</p>";
    } else {
      html += "<p>Location not available</p>";
    }

    html += "</body></html>";
    request->send(200, "text/html", html);
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

    // Activeer de zoemer
    digitalWrite(buzzerPin, HIGH);
    delay(100);

    // Voeg eventuele extra acties toe die je wilt uitvoeren wanneer de knop wordt ingedrukt
    delay(500);

    // Deactiveer de zoemer
    digitalWrite(buzzerPin, LOW);
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
      delay(1000);
    }
  }
}
