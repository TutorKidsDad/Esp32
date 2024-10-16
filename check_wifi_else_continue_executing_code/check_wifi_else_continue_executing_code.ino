#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* server = "api.thingspeak.com";
const String apiKey = "YourAPIKey";

const int interval = 5000; // interval between sensor readings (in milliseconds)
unsigned long previousMillis = 0; // variable to store last time sensor was read

const int LED_1_PIN = 4; // Pin number for LED 1
const int LED_2_PIN = 5; // Pin number for LED 2

void setup() {
  Serial.begin(9600);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  WiFi.begin(ssid, password);

  // Wait for WiFi connection for 30 seconds
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) {
    digitalWrite(LED_2_PIN, HIGH); // Turn on LED 2
    delay(1000);
    digitalWrite(LED_2_PIN, LOW); // Turn off LED 2
    delay(1000);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    digitalWrite(LED_1_PIN, HIGH); // Turn on LED 1
  } else {
    Serial.println("WiFi not connected, continuing anyway...");
    digitalWrite(LED_2_PIN, HIGH); // Turn on LED 2
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Code to read sensor data goes here
    float sensorValue = 0.0; // replace with your actual sensor reading

    // Construct URL for ThingSpeak API POST request
    String url = "/update?api_key=";
    url += apiKey;
    url += "&field1=";
    url += String(sensorValue);

    // Make HTTP POST request to ThingSpeak
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
  }

  // Other code to be executed goes here
  // ...
}
