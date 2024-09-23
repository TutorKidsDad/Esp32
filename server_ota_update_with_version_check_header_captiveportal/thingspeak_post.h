#ifndef THINGSPEAK_POST_H
#define THINGSPEAK_POST_H

#include <HTTPClient.h>
#include <WiFi.h>


// Define your ThingSpeak Channel ID and API Key
const char* THINGSPEAK_API_KEY = "YOUR_API_KEY";  // Replace with your ThingSpeak Write API Key
const char* THINGSPEAK_SERVER = "http://api.thingspeak.com";

// Function to post data to ThingSpeak
void postDataToThingSpeak(float temperature, float humidity, int analogValue) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String url = String(THINGSPEAK_SERVER) + "/update?api_key=" + THINGSPEAK_API_KEY +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(humidity) +
                 "&field3=" + String(analogValue);
    
    http.begin(url); // Initialize the HTTP connection
    int httpCode = http.GET(); // Send HTTP GET request
    
    if (httpCode > 0) {
      Serial.printf("ThingSpeak POST Response Code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.printf("POST failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end(); // Close the connection
  } else {
    Serial.println("WiFi not connected, unable to post to ThingSpeak.");
  }
}

#endif // THINGSPEAK_POST_H
