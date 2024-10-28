#include <Arduino.h>
#include "ota_update.h"  // OTA header
#include "dht_sensor.h"  // DHT11 sensor header
#include <WiFiManager.h> // WiFiManager library

void setup() {
  Serial.begin(115200);

  // Initialize the DHT sensor
  initDHT();

  // Connect to WiFi using WiFiManager
  WiFiManager wm;
  if (!wm.autoConnect("AutoConnectAP")) { // Starts an access point with SSID "AutoConnectAP"
    Serial.println("Failed to connect and hit timeout. Going to deep sleep.");
    goToDeepSleep();
  }
  // Successfully connected
  Serial.println("Connected to WiFi.");

  // Check for OTA updates if connected
  if (WiFi.status() == WL_CONNECTED) {
    checkForUpdate();
  } else {
    goToDeepSleep();
  }
}

void loop() {
  // Read analog value from pin 34
  int analogValue = analogRead(34);
  Serial.printf("Analog value from pin 34: %d\n", analogValue);

  // Read temperature and humidity from DHT11 sensor
  float temperature = readTemperature();
  float humidity = readHumidity();
  
  Serial.printf("Temperature: %.2f°C\n", temperature);
  Serial.printf("Humidity: %.2f%%\n", humidity);

  delay(2000); // Delay between readings
}
