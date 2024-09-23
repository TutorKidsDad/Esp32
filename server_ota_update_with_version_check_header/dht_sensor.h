#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <DHT.h>

#define DHTPIN 14     // Pin where the DHT11 data pin is connected
#define DHTTYPE DHT11 // DHT 11 sensor

// Create a DHT object
DHT dht(DHTPIN, DHTTYPE);

// Function to initialize the DHT sensor
void initDHT() {
  dht.begin();
  Serial.println("DHT11 sensor initialized.");
}

// Function to read temperature from DHT11 sensor
float readTemperature() {
  float temp = dht.readTemperature(); // Reading temperature as Celsius
  if (isnan(temp)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return -1;
  }
  return temp;
}

// Function to read humidity from DHT11 sensor
float readHumidity() {
  float humidity = dht.readHumidity();
  if (isnan(humidity)) {
    Serial.println("Failed to read humidity from DHT sensor!");
    return -1;
  }
  return humidity;
}

#endif // DHT_SENSOR_H
