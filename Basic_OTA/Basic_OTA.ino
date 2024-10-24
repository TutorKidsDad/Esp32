#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h> // Include WiFiManager library

// Create an instance of WiFiManager
WiFiManager wifiManager;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  // Set WiFi mode to Station
  WiFi.mode(WIFI_STA);

  // Automatically connect using previously saved credentials or open the captive portal
  if (!wifiManager.autoConnect("ESP32_AP")) {
    Serial.println("Failed to connect to Wi-Fi. Rebooting...");
    delay(3000);
    ESP.restart();
  }

  // Set fixed hostname "sensor1"
  const char* hostname = "sensor1";

  // Start mDNS service
  if (!MDNS.begin(hostname)) {
    Serial.println("Error setting up mDNS responder!");
    while (1) {
      delay(1000); // Stop if mDNS setup fails
    }
  }
  Serial.println("mDNS responder started");

  // Print the hostname to access the ESP32
  Serial.print("You can access the ESP32 at http://");
  Serial.print(hostname);
  Serial.println(".local");

  // Set OTA configurations
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword("your_secure_password");

  // Configure OTA callbacks
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH) {
        type = "sketch";
      } else { // U_SPIFFS
        type = "filesystem";
      }
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Handle OTA update requests
  ArduinoOTA.handle();

  // Monitor free heap memory
  Serial.printf("Free Heap: %d\n", ESP.getFreeHeap());

  // Optional: Deep sleep logic based on your project requirements
  // Example: deep sleep after 5 minutes of inactivity or other criteria
  // if (millis() > 300000) {
  //   Serial.println("Entering deep sleep...");
  //   ESP.deepSleep(600e6); // Sleep for 10 minutes
  // }
}
