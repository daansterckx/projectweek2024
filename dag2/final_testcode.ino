#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <Adafruit_PCD8544.h>

// The serial connection to the GPS module
SoftwareSerial ss(4, 5);
TinyGPSPlus gps;
#include <Ultrasonic.h>


const int echoPin = 6;
const int trigPin = 7;
const int buzzerPin = 2;
const int buzzerPinAssemble = 3;
const int helpbutton = 1;
const int assumblecuntsbutton = 0;

Ultrasonic ultrasonic(trigPin, echoPin);
// Nokia 5110 display pins
#define PIN_CLK 9
#define PIN_DIN 10
#define PIN_DC 11
#define PIN_RST 12
#define PIN_CS 13

Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_CLK, PIN_DIN, PIN_DC, PIN_RST, PIN_CS);

void setup(){
    Serial.begin(9600);
    ss.begin(9600);
    pinMode(buzzerPin, OUTPUT);
    

    display.begin();
    display.setContrast(50);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(BLACK);
    display.display();
}

void loop(){
    while (ss.available() > 0){
        long distance = ultrasonic.read();
        if (distance < 3) {
            digitalWrite(buzzerPin, HIGH);
            delay(500);
            digitalWrite(buzzerPin, LOW);
            Serial.println(distance);
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("Kind in");
            display.setCursor(0, 8);
            display.println("verboden zone!");
            display.display();
        }
        while (digitalRead(assumblecuntsbutton) == LOW) {
            digitalWrite(buzzerPinAssemble, HIGH);
            delay(500);
            digitalWrite(buzzerPinAssemble, LOW);
            Serial.println(distance);
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("verzamelen!!");
            display.display();
        }
        while (digitalRead(helpbutton) == LOW) {
            digitalWrite(buzzerPin, HIGH);
            delay(500);
            digitalWrite(buzzerPin, LOW);
            Serial.println(distance);
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("kind in ");
            display.setCursor(0, 8);
            display.println("nood!");
            display.display();
        }
        // Parse the incoming byte data from the GPS
        if (gps.encode(ss.read())) {
            // If a valid location is available, print it
            if (gps.location.isValid()) {
                display.clearDisplay();
                display.setCursor(0, 0);
                display.println("Latitude:");
                display.println(gps.location.lat(), 6);
                display.println("Longitude:");
                display.println(gps.location.lng(), 6);
                display.display();

                Serial.print("Latitude: ");
                Serial.println(gps.location.lat(), 6);
                Serial.print("Longitude: ");
                Serial.println(gps.location.lng(), 6);
            } else {
                display.clearDisplay();
                display.setCursor(0, 0);
                display.println("Location:");
                display.println("Not Available");
                display.display();

                Serial.println("Location: Not Available");
            }
            delay(500);
        }
    }
}