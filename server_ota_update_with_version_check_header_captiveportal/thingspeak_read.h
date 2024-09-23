#ifndef THINGSPEAK_READ_H
#define THINGSPEAK_READ_H

#include <HTTPClient.h>
#include <WiFi.h>
#include <ArduinoJson.h> // For parsing JSON responses

// Replace with your ThingSpeak Channel ID and Read API Key
const char* THINGSPEAK_READ_API_KEY = "YOUR_READ_API_KEY";   // ThingSpeak Read API Key
const char* THINGSPEAK_SERVER_READ = "http://api.thingspeak.com";
const int THINGSPEAK_CHANNEL_ID = 1234567; // Replace with the channel ID you want to read from

// Function to read data from ThingSpeak and print to Serial Monitor
void readDataFromThingSpeak() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Construct the URL for ThingSpeak GET request
    String url = String(THINGSPEAK_SERVER_READ) + "/channels/" + String(THINGSPEAK_CHANNEL_ID) +
                 "/feeds.json?api_key=" + THINGSPEAK_READ_API_KEY + "&results=1";  // Fetch the most recent data

    http.begin(url); // Initialize HTTP request
    int httpCode = http.GET(); // Send the request
    
    if (httpCode > 0) {
      // Check if the request was successful (HTTP code 200 OK)
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();  // Get the response as a string
        Serial.println("Received data from ThingSpeak:");
        Serial.println(payload);  // Print raw JSON response

        // Parse JSON data to get specific fields
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);

        // Access the latest feed data for three fields
        float field1_value = doc["feeds"][0]["field1"]; // Field 1
        float field2_value = doc["feeds"][0]["field2"]; // Field 2
        float field3_value = doc["feeds"][0]["field3"]; // Field 3

        Serial.printf("Field 1 Value: %.2f\n", field1_value);
        Serial.printf("Field 2 Value: %.2f\n", field2_value);
        Serial.printf("Field 3 Value: %.2f\n", field3_value);
      }
    } else {
      Serial.printf("Failed to connect to ThingSpeak: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end(); // Close the connection
  } else {
    Serial.println("WiFi not connected, unable to read from ThingSpeak.");
  }
}

#endif // THINGSPEAK_READ_H
