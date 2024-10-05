#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Example code for the Adafruit TCS34725 color sensor library

// Connection Instructions:
// - Connect SCL to analog pin 5
// - Connect SDA to analog pin 4
// - Connect VDD to 3.3V DC
// - Connect GROUND to common ground

// Initialize the TCS34725 sensor with specific integration time and gain
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(115200); // Initialize serial communication at 115200 baud

  // Attempt to initialize the sensor
  if (tcs.begin()) {
    Serial.println("Color sensor initialized successfully.");
  } else {
    Serial.println("Error: No TCS34725 sensor found! Please check your connections.");
    while (1); // Halt execution if sensor initialization fails
  }

  // Ready to get readings!
  Serial.println("Starting to read color data...");
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;

  // Get raw RGB and clear light readings
  tcs.getRawData(&r, &g, &b, &c);
  
  // Calculate color temperature and lux from raw readings
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  // Print the results to the Serial Monitor in a user-friendly format
  Serial.println("Color Sensor Readings:");
  Serial.print("  Color Temperature: "); 
  Serial.print(colorTemp); 
  Serial.println(" K");
  
  Serial.print("  Illuminance (Lux): "); 
  Serial.print(lux); 
  Serial.println(" Lux");
  
  Serial.print("  Raw Values - R: "); 
  Serial.print(r); 
  Serial.print(", G: "); 
  Serial.print(g); 
  Serial.print(", B: "); 
  Serial.print(b); 
  Serial.print(", C: "); 
  Serial.print(c); 
  Serial.println(" ");
  
  delay(300); // Delay to avoid flooding the Serial Monitor with data
}
