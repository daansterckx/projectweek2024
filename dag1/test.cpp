#include <TinyGPS++.h>
#include <HardwareSerial.h>

// The serial connection to the GPS module
HardwareSerial ss(2);

TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);
  hs.begin(9600, SERIAL_8N1, 18, 19); // RX=16, TX=17
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
    }
  }
}