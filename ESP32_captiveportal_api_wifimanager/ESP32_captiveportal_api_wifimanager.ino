#include <WiFi.h>
#include <WiFiManager.h>  // Include the WiFiManager library
#include <HTTPClient.h>

// Declare global variables for API key storage
char apiKey[40] = ""; // Large enough to store an API key
WiFiManager wifiManager;

// Pin definitions
const int LED_1_PIN = 4; // Pin number for LED 1
const int LED_2_PIN = 5; // Pin number for LED 2

// Interval between sensor readings
const int interval = 5000; // interval in milliseconds
unsigned long previousMillis = 0;

// Server information
const char* server = "api.thingspeak.com";

void setup() {
  Serial.begin(9600);
  
  // Initialize LED pins
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  
  // Create a custom WiFiManager parameter to input the API key
  WiFiManagerParameter customApiKey("apiKey", "Enter API Key", apiKey, 40);
  wifiManager.addParameter(&customApiKey);

  // AutoConnect will attempt to connect to the last known network or open a captive portal
  if (!wifiManager.autoConnect("ESP32_AP")) {
    Serial.println("Failed to connect and hit timeout");
    ESP.restart();
  }

  // Save the entered API key in EEPROM or use a persistent storage if needed
  strcpy(apiKey, customApiKey.getValue());
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Print the API key for verification
  Serial.print("API Key: ");
  Serial.println(apiKey);

  // Blink LED 1 to indicate successful connection
  digitalWrite(LED_1_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_1_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Code to read sensor data goes here
    float sensorValue = 0.0; // replace with your actual sensor reading

    // Construct URL for ThingSpeak API POST request
    String url = "/update?api_key=";
    url += apiKey; // Use the user-provided API key
    url += "&field1=";
    url += String(sensorValue);

    // Make HTTP POST request to ThingSpeak
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;
      http.begin(client, server, 80, url);
      int httpResponseCode = http.POST("");
      
      if (httpResponseCode == 200) {
        Serial.println("Sensor data successfully posted to ThingSpeak");
      } else {
        Serial.print("Error posting sensor data to ThingSpeak, error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("Wi-Fi not connected. Unable to post data.");
    }
  }
  
  // Other code to be executed goes here
  // ...
}

