#ifndef OTA_UPDATE_H
#define OTA_UPDATE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <esp_sleep.h>

// Define the current firmware version of your ESP32
const String currentVersion = "v1.0.2";

// URL to check for the latest version and download firmware
const char* version_url = "http://www.tweenz.in/version.txt";
const char* firmware_url = "http://www.tweenz.in/a.bin";

// Time to sleep (in microseconds) => 5 minutes
#define uS_TO_S_FACTOR 1000000  // Conversion factor for seconds to microseconds
#define TIME_TO_SLEEP  300  // Time ESP32 will sleep in seconds (5 minutes)

void otaUpdate();

// Function to connect to WiFi
void connectToWiFi(const char* ssid, const char* password) {
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();

  // Attempt to connect to WiFi for 30 seconds
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 30000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi");
  }
}

// Function to check for updates
void checkForUpdate() {
  HTTPClient http;

  // Check the latest version on the server
  Serial.println("Checking for new firmware version...");

  http.begin(version_url); // URL that returns latest version
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String latestVersion = http.getString();
    latestVersion.trim();  // Remove extra spaces or newlines

    Serial.printf("Current version: %s, Latest version: %s\n", currentVersion.c_str(), latestVersion.c_str());

    if (latestVersion.equals(currentVersion)) {
      Serial.println("Firmware is up to date.");
      return; // No update needed
    } else {
      Serial.println("New version available, starting OTA update...");
      otaUpdate(); // New version is available, proceed with OTA
    }
  } else {
    Serial.printf("Error fetching version: %d\n", httpCode);
  }

  http.end();
}

// Function to perform OTA update
void otaUpdate() {
  HTTPClient http;

  Serial.println("Starting OTA update...");

  http.begin(firmware_url);
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    int contentLength = http.getSize();
    WiFiClient *client = http.getStreamPtr();

    if (contentLength > 0) {
      if (!Update.begin(contentLength)) {
        Update.printError(Serial);
        return;
      }

      // OTA Update Write process
      size_t written = 0;
      uint8_t buff[128] = { 0 };
      while (http.connected() && written < contentLength) {
        size_t available = client->available();
        if (available) {
          int readBytes = client->readBytes(buff, sizeof(buff));
          written += Update.write(buff, readBytes);
          Serial.printf("Written: %u of %u bytes\n", written, contentLength);
        }
        delay(1);
      }

      if (Update.end()) {
        Serial.println("Update Success! Rebooting...");
        ESP.restart();
      } else {
        Update.printError(Serial);
      }
    } else {
      Serial.println("No content in response!");
    }
  } else {
    Serial.printf("HTTP error code: %d\n", httpCode);
  }

  http.end();
}

// Function to go into deep sleep
void goToDeepSleep() {
  Serial.println("Going to deep sleep for 5 minutes...");
  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

#endif // OTA_UPDATE_H
