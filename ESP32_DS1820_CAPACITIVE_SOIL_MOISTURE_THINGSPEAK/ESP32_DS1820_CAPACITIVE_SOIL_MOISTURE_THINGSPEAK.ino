#ifdef ESP8266
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include <SPI.h>
#include <Wire.h>
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Define GPIO pin for DS18B20 sensor
#define ONE_WIRE_BUS 4 // GPIO pin for DS18B20 on ESP32 (Change if necessary)
#define INTERVAL 15000  // 15 seconds interval (in milliseconds)

// Initialize OneWire and DallasTemperature instances
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

// ThingSpeak API settings
String apiKey = "";  // Enter your Write API key from ThingSpeak
const char *ssid = "OnlineTPS";  // WiFi SSID
const char *pass = "TPS@462014"; // WiFi Password
const char* server = "api.thingspeak.com";  // ThingSpeak server

// Soil moisture sensor pin
const int sensor_pin = A0;  // Connect Soil moisture analog sensor pin to A0

// WiFi client
WiFiClient client;

// Track the time of the last data upload
unsigned long lastUploadTime = 0;

// Function to connect to WiFi
void connectWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
}

void setup() {
  // Start serial communication
  Serial.begin(115200);
  
  // Connect to WiFi
  connectWiFi();

  // Initialize DS18B20 sensor
  DS18B20.begin();
}

void loop() {
  // Check if enough time has passed (15 seconds)
  unsigned long currentMillis = millis();
  if (currentMillis - lastUploadTime >= INTERVAL) {
    
    // Update last upload time
    lastUploadTime = currentMillis;

    // Read temperature from DS18B20 sensor
    float temp;
    DS18B20.requestTemperatures();
    temp = DS18B20.getTempCByIndex(0);

    Serial.print("Temperature: ");
    Serial.println(temp);

    // Read soil moisture value
    int moisture_value = analogRead(sensor_pin); // Reading analog value from soil moisture sensor

    // Convert the analog reading (0-1023) to a percentage
    int moisture_percentage = (100 - ((moisture_value / 4095.0) * 100.0));  // ESP32 uses 12-bit ADC (0-4095)

    Serial.print("Soil Moisture (in Percentage) = ");
    Serial.print(moisture_percentage);
    Serial.println("%");

    // Send data to ThingSpeak if connected to the server
    if (client.connect(server, 80)) {
      String postStr = apiKey;
      postStr += "&field7=";
      postStr += String(moisture_percentage);
      postStr += "&field8=";
      postStr += String(temp);
      postStr += "\r\n\r\n";

      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);

      Serial.println("Data Sent to ThingSpeak");
    }

    // Disconnect from the server
    client.stop();
    Serial.println("Waiting for next update...");
  }
}
