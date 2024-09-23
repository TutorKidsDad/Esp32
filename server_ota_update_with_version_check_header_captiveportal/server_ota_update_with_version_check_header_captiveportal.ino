#include <Arduino.h>
#include "ota_update.h"  // OTA header
#include "dht_sensor.h"  // DHT11 sensor header
#include "thingspeak_post.h" // ThingSpeak post header
#include "thingspeak_read.h" // ThingSpeak read header
#include "pins.h"
#include "display.h"

void setup() {
  Serial.begin(115200);

  // Initialize the DHT sensor
  initDHT();
init_display();
  // Connect to WiFi using captive portal
  connectToWiFi();
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
  
 // Post data to ThingSpeak
  postDataToThingSpeak(temperature, humidity, analogValue);

  // Read data from another ThingSpeak channel and print to Serial
  readDataFromThingSpeak();
  
  delay(20000); // Delay between data posts
}
