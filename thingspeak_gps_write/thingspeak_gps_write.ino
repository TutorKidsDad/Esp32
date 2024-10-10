#include <TinyGPSPlus.h>
#include <Wire.h>  // Include Wire for I2C communication, if needed
#include "ThingSpeak.h"
#if defined(ESP32)
#include <WiFi.h> // Include for ESP32
static const int RXPin = 16; // Change according to your setup for ESP32
static const int TXPin = 17; // Change according to your setup for ESP32
#else // Assume ESP8266 if not ESP32
#include <ESP8266WiFi.h> // Include for ESP8266
static const int RXPin = 12; // RX pin for ESP8266
static const int TXPin = 13; // TX pin for ESP8266
#endif

// Replace your Wi-Fi SSID and password
const char* ssid     = "testing"; // SSID
const char* password = "password"; // Password

// ThingSpeak Channel Number and API Key
unsigned long myChannelNumber = 617957;
const char* myWriteAPIKey = ""; // Put your ThingSpeak Write API Key here

// The TinyGPS++ object
TinyGPSPlus gps;
WiFiClient client;

// The serial connection to the GPS device (using hardware serial for ESP32)
#if defined(ESP32)
#define SerialGPS Serial1 // Use Serial1 for ESP32 (change as needed)
#else
#include <SoftwareSerial.h>
SoftwareSerial ss(RXPin, TXPin); // SoftwareSerial for ESP8266
#endif

void setup() {
  Serial.begin(115200);
  #if defined(ESP32)
  SerialGPS.begin(9600); // Set baud rate for GPS on Serial1
  #else
  ss.begin(9600); // Set baud rate for GPS on SoftwareSerial
  #endif
  
  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  
  delay(500);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Netmask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  
  ThingSpeak.begin(client);
  delay(500);
}

void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  
  // Read GPS data from SoftwareSerial for ESP8266 or from the Serial for ESP32
  #if defined(ESP32)
  // ESP32 uses the hardware serial directly (usually Serial)
  while (Serial.available() > 0) {
    if (gps.encode(Serial.read())) {
      displayInfo();
    }
  }
  #else
  // ESP8266 reads from SoftwareSerial
  while (ss.available() > 0) {
    if (gps.encode(ss.read())) {
      displayInfo();
    }
  }
  #endif
  
  delay(500);
  
  // Check if GPS is detected
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }

  delay(500);
}


void displayInfo() {
  if (gps.location.isValid()) {
    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    
    String latbuf;
    latbuf += (String(latitude, 6));
    Serial.println(latbuf);

    String lonbuf;
    lonbuf += (String(longitude, 6));
    Serial.println(lonbuf);

    ThingSpeak.setField(1, latbuf);
    ThingSpeak.setField(2, lonbuf);
    ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    delay(20000); // Send data every 20 seconds
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid()) {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  } else {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid()) {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  } else {
    Serial.print(F("INVALID"));
    delay(500);
  }

  Serial.println();
  delay(1500);
}
