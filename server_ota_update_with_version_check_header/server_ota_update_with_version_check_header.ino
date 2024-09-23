#include <Arduino.h>
#include "ota_update.h"  // OTA header
#include "dht_sensor.h"  // DHT11 sensor header

// Replace with your network credentials
const char* ssid = "swapnil2@4";
const char* password = "TPS@462016";

void setup() {
  Serial.begin(115200);

  // Initialize the DHT sensor
  initDHT();

  // Connect to WiFi and check for OTA updates
  connectToWiFi(ssid, password);
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
