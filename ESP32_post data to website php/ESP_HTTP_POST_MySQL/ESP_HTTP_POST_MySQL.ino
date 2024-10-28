#ifdef ESP32
  #include <WiFi.h>
  #include <HTTPClient.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiClient.h>
#endif

#include <WiFiManager.h> // Include WiFiManager

// Domain name with URL path or IP address with path
const char* serverName = "http://onlinetps.com/test/esp-post-data.php";

// API Key value to be compatible with the PHP code provided in the project page.
String apiKeyValue = "tPmAT5Ab3j7F9";
String sensorName = "BME280";
String sensorLocation = "Office";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000; // Send data every 30 seconds

void setup() {
  // Start Serial communication
  Serial.begin(115200);

  // Initialize WiFiManager
  WiFiManager wm;

  // Set timeout for the WiFiManager (e.g., 3 minutes to connect)
  wm.setTimeout(180); 

  // Try to connect, and if it fails, it will create a new portal for configuration
  if (!wm.autoConnect("ESP_AutoConnectAP")) {
    Serial.println("Failed to connect and hit timeout");
    // Reset and try again, or enter deep sleep
    ESP.restart();
  }

  // If you get here, you have connected to Wi-Fi successfully
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 30 seconds (timerDelay variable), it will take 30 seconds before publishing the first reading.");
}

void loop() {
  // Send an HTTP POST request every timerDelay
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      String jsonPayload = "{\"api_key\":\"" + apiKeyValue + "\",\"sensor\":\"" + sensorName + "\","
                           "\"location\":\"" + sensorLocation + "\",\"value1\":\"" + String(100) + "\","
                           "\"value2\":\"" + String(200) + "\",\"value3\":\"" + String(300) + "\"}";
      http.addHeader("Content-Type", "application/json");

      // Send HTTP POST request
      int httpResponseCode = http.POST(jsonPayload);

      // Print the HTTP response code
      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }

      // Free resources
      http.end();
    } else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
