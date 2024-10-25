#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <WiFiManager.h>

// Define your current firmware version
#define CURRENT_FIRMWARE_VERSION "1.0.0"

// URL of the firmware binary file
const char* firmwareUrl = "http://your_website.com/firmware.bin";

void performOTAUpdate() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(firmwareUrl);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      int contentLength = http.getSize();
      if (contentLength > 0) {
        // Extract firmware version from the HTTP header (or use a specific URL query if necessary)
        String firmwareVersion = http.header("X-Firmware-Version");

        Serial.print("Current firmware version: ");
        Serial.println(CURRENT_FIRMWARE_VERSION);
        Serial.print("New firmware version: ");
        Serial.println(firmwareVersion);

        // Compare the versions
        if (firmwareVersion != CURRENT_FIRMWARE_VERSION) {
          Serial.println("Updating firmware...");
          if (Update.begin(contentLength)) {
            size_t written = Update.writeStream(http.getStream());
            if (written == contentLength) {
              Serial.println("Firmware updated successfully. Rebooting...");
              Update.end(true);
              ESP.restart();
            } else {
              Serial.printf("Error: Failed to write firmware. Written %d of %d bytes\n", written, contentLength);
              Update.end(false);
            }
          } else {
            Serial.println("Error: Not enough space for OTA update");
          }
        } else {
          Serial.println("Firmware is already up-to-date");
        }
      } else {
        Serial.println("Error: Empty firmware binary");
      }
    } else {
      Serial.printf("Error: Failed to download firmware binary. HTTP code: %d\n", httpCode);
    }
    http.end();
  } else {
    Serial.println("Error: Not connected to Wi-Fi");
  }
}

void setup() {
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.autoConnect("OTA-Setup-AP");
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi successfully.");
  } else {
    Serial.println("Failed to connect to Wi-Fi.");
  }
}

void loop() {
  performOTAUpdate();
  delay(60000);
}
