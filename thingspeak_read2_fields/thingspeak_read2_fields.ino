#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Replace with your actual WiFi credentials
#define WIFI_SSID ""
#define WIFI_PASS "" 
// Replace with your actual ThingSpeak API credentials
#define THINGSPEAK_CHANNEL_ID ""
#define THINGSPEAK_API_KEY ""

// Variables to hold field values
float field1Value = 0.0;
float field2Value = 0.0;

// Function to connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

// Function to fetch data from ThingSpeak
void fetchDataFromThingSpeak() {
  HTTPClient http;

  // Construct the URL for the GET request
  String url = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID) + "/feeds/last.json?api_key=" + String(THINGSPEAK_API_KEY);
  Serial.println("Fetching data from URL: " + url);

  // Start the HTTP request
  http.begin(url);
  int httpCode = http.GET();

  // Check the returning code
  if (httpCode > 0) {
    // HTTP header has been sent and server response header has been handled
    Serial.printf("HTTP GET request code: %d\n", httpCode);

    // File found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Received payload:");
      Serial.println(payload);

      // Parse JSON payload
      DynamicJsonDocument doc(1024); // Adjust size as needed
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        // Extract the value from field1 and field2
        field1Value = doc["field1"].as<float>();
        field2Value = doc["field2"].as<float>();

        Serial.println("Field1: " + String(field1Value));
        Serial.println("Field2: " + String(field2Value));
      } else {
        Serial.print("Failed to parse JSON, error: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.println("Failed to fetch data");
    }
  } else {
    Serial.printf("Failed to make request, error: %s\n", http.errorToString(httpCode).c_str());
  }

  // Close the connection
  http.end();
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Give time for the serial monitor to open
  connectToWiFi();
  fetchDataFromThingSpeak();
}

void loop() {
  // Example: fetch data every 30 seconds
  delay(30000);
  fetchDataFromThingSpeak();
}
