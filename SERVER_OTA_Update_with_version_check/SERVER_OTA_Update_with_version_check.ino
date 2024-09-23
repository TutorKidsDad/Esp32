#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <esp_sleep.h> // Include for deep sleep functionality

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Define the current firmware version of your ESP32
const String currentVersion = "v1.0.0";

// URL to check for the latest version and download firmware
const char* version_url = "http://www.tweenz.in/version.txt";
const char* firmware_url = "http://www.tweenz.in/a.bin";

// Time to sleep (in microseconds) => 5 minutes
#define uS_TO_S_FACTOR 1000000  // Conversion factor for seconds to microseconds
#define TIME_TO_SLEEP  300  // Time ESP32 will sleep in seconds (5 minutes)

void setup() {
  Serial.begin(115200);

  // Attempt to connect to Wi-Fi
  connectToWiFi();

  // Check for OTA update if connected
  if (WiFi.status() == WL_CONNECTED) {
    checkForUpdate();
  } else {
    Serial.println("WiFi not connected, going to deep sleep for 5 minutes...");
    goToDeepSleep();
  }
}

void connectToWiFi() {
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
      // Proceed with the rest of your program here
      return; // No update needed, continue execution
    } else {
      Serial.println("New version available, starting OTA update...");
      otaUpdate(); // New version is available, proceed with OTA
    }
  } else {
    Serial.printf("Error fetching version: %d\n", httpCode);
  }

  http.end();
}

void otaUpdate() {
  HTTPClient http;

  Serial.println("Starting OTA update...");

  // Begin the OTA update process by downloading the binary
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

      // Check if the OTA update is done
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

// Function to go into deep sleep for 5 minutes
void goToDeepSleep() {
  Serial.println("Going to deep sleep for 5 minutes...");
  
  // Configure the timer to wake the ESP32 after TIME_TO_SLEEP seconds
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  // Enter deep sleep mode
  esp_deep_sleep_start();
}

void loop() {
  // Normal program code goes here
}
