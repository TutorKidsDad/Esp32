/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/control-esp32-esp8266-gpios-from-anywhere/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiManager.h>  // Include WiFiManager library
#include <ESPmDNS.h>      // Include mDNS library

const char* serverName = "http://onlinetps.com/test1/esp-outputs-action.php?action=outputs_state&board=1";

// Update interval time set to 5 seconds
const long interval = 5000;
unsigned long previousMillis = 0;

String outputsState;

void setup() {
  Serial.begin(115200);

  // Create an instance of WiFiManager
  WiFiManager wm;

  // Configure Access Point with a specific IP and password
  IPAddress apIP(192, 168, 4, 1);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  wm.setAPStaticIPConfig(apIP, gateway, subnet);

  // Set timeout for configuration portal
  wm.setConfigPortalTimeout(180); // 3 minutes to connect before timeout
  
  // Open the captive portal with a custom SSID and password
  if (!wm.autoConnect("ESP32-Setup", "YOUR_PORTAL_PASSWORD")) {
    Serial.println("Failed to connect, entering deep sleep mode...");
    esp_deep_sleep_start();  // Enter deep sleep if Wi-Fi connection fails
  }

  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS responder with a friendly name "sensor1"
  if (!MDNS.begin("sensor1")) {
    Serial.println("Error setting up mDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started. You can access the ESP32 at http://sensor1.local");
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) { 
      outputsState = httpGETRequest(serverName);
      Serial.println(outputsState);
      JSONVar myObject = JSON.parse(outputsState);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
    
      Serial.print("JSON object = ");
      Serial.println(myObject);
    
      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();
    
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = myObject[keys[i]];
        Serial.print("GPIO: ");
        Serial.print(keys[i]);
        Serial.print(" - SET to: ");
        Serial.println(value);
        pinMode(atoi(keys[i]), OUTPUT);
        digitalWrite(atoi(keys[i]), atoi(value));
      }
      // save the last HTTP GET Request
      previousMillis = currentMillis;
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  }
}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
  
  // Send HTTP GET request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
