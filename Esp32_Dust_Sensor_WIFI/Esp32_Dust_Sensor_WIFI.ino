#include <WiFiManager.h>  // WiFiManager library
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>      // Include for mDNS

// Constants and Pin Definitions
#define COV_RATIO            0.2      // Conversion factor: µg/m³ per mV
#define NO_DUST_VOLTAGE      400      // Baseline voltage in mV when no dust is detected
#define SYSTEM_VOLTAGE       5000     // System voltage in mV

const int ledPin = 12;                 // LED control pin for the dust sensor
const int sensorPin = 36;              // Analog input pin for the sensor
float dustDensity = 0;
int adcValue;

// Function Prototypes
int filterADC(int measurement);
void updateDustSensorValue();
void sendDustDataToServer();

// Setup Function
void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // Default state: LED off
  
  Serial.begin(115200);
  
  // Set up WiFiManager
  WiFiManager wifiManager;
  
  // Optionally add a password for the captive portal
  wifiManager.setConfigPortalTimeout(180); // 3 minutes timeout for captive portal
  wifiManager.autoConnect("ESP32_Sensor", "sensor123"); // SSID and password for the captive portal

  // After auto-connect, if Wi-Fi is not connected, enter deep sleep
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to Wi-Fi. Entering deep sleep...");
    ESP.deepSleep(60 * 1000000); // Sleep for 1 minute before rebooting
  }
  
  // Initialize mDNS with the chosen hostname
  if (!MDNS.begin("sensor1")) { // Access ESP32 at http://sensor1.local
    Serial.println("Error starting mDNS");
  } else {
    Serial.println("mDNS responder started as 'sensor1.local'");
  }
}

// Main Loop
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    updateDustSensorValue();
    sendDustDataToServer();
  } else {
    Serial.println("Wi-Fi Disconnected. Reconnecting using WiFiManager...");
    WiFi.reconnect();
  }
  
  delay(2000); // Wait for 2 seconds before the next iteration
}

// Function to Update Dust Sensor Value
void updateDustSensorValue() {
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(280); // Wait for stable reading
  adcValue = analogRead(sensorPin);
  digitalWrite(ledPin, LOW);
  
  adcValue = filterADC(adcValue); // Apply moving average filter
  
  // Convert ADC value to voltage in mV
  float voltage = (SYSTEM_VOLTAGE / 4096.0) * adcValue * 11.0; // 4096 for 12-bit ADC resolution
  
  // Calculate dust density from voltage
  if (voltage >= NO_DUST_VOLTAGE) {
    voltage -= NO_DUST_VOLTAGE;
    dustDensity = voltage * COV_RATIO;
  } else {
    dustDensity = 0;
  }
  
  // Display the result
  Serial.print("Dust Concentration: ");
  Serial.print(dustDensity);
  Serial.println(" µg/m³");
}

// Function to Filter ADC Values (Moving Average Filter)
int filterADC(int measurement) {
  static int initialized = 0, buffer[10], sum;
  const int bufferSize = 10;

  if (!initialized) {
    initialized = 1;
    sum = measurement * bufferSize;
    for (int i = 0; i < bufferSize; i++) {
      buffer[i] = measurement;
    }
    return measurement;
  }

  // Update the buffer and recalculate the sum
  sum -= buffer[0];
  for (int i = 0; i < bufferSize - 1; i++) {
    buffer[i] = buffer[i + 1];
  }
  buffer[bufferSize - 1] = measurement;
  sum += buffer[bufferSize - 1];

  // Return the average value
  return sum / bufferSize;
}

// Function to Send Dust Data to Server
void sendDustDataToServer() {
  HTTPClient http;
  String serverURL = "https://dust-sensor-cloud.firebaseapp.com/api/v1/sensor/9vga20P652xhl6pgLidS";

  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");
  
  // Prepare JSON payload
  StaticJsonDocument<200> jsonDoc;
  jsonDoc["dust"] = dustDensity;
  
  String jsonData;
  serializeJson(jsonDoc, jsonData);

  int httpResponseCode = http.PATCH(jsonData);

  if (httpResponseCode > 0) {
    Serial.print("Data sent successfully, Response Code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error in sending data. HTTP Response Code: ");
    Serial.println(httpResponseCode);
  }

  http.end(); // Clean up
}
