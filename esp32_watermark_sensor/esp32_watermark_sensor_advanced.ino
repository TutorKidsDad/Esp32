#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>  // Include mDNS library

// Number of sensor reads for filtering
#define NUM_READS 11    

// Structure to store moisture and resistance values
typedef struct {        
  int moisture;
  long resistance;
} MoistureData;

// Constants
const long RESISTOR_VALUE = 4700;  // Resistor value in Ohms
int activePin = 18;                // Active pin for current paths
int supplyPin;                     // Pin for supply voltage
int sensorPin;                     // Pin for sensor voltage
int supplyVoltage;                 // Supply voltage value
int sensorVoltage;                 // Sensor voltage value
MoistureData readings[NUM_READS]; // Stores readings for filtering

// LCD initialization (I2C address 0x27 for 16x2 display)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

// Create a web server on port 80
WebServer server(80); 

// Setup function
void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize I2C LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture");

  // Initialize ESP32 pins
  pinMode(18, OUTPUT);  // Digital pin 18 for path 1
  pinMode(19, OUTPUT);  // Digital pin 19 for path 2
  delay(200);

  // Set up ESP32 as a Wi-Fi Access Point
  WiFi.softAP("SoilMoistureAP", "password"); // Create AP with SSID and password
  Serial.println("Access Point started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP()); // Print the IP address of the AP

  // Set up mDNS
  if (MDNS.begin("soilmoisture")) { // Set hostname
    Serial.println("MDNS responder started");
  }

  // Define the server routes
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();

  // Read sensor values and calculate resistance
  for (int i = 0; i < NUM_READS; i++) {
    switchCurrentPath();  // Configure pins for measurement

    // Turn on voltage supply for the sensor
    digitalWrite(activePin, HIGH);
    delay(25);
    supplyVoltage = analogRead(supplyPin);  // Read supply voltage
    sensorVoltage = analogRead(sensorPin);  // Read sensor voltage
    digitalWrite(activePin, LOW);  // Turn off voltage supply
    delay(25);

    // Calculate resistance and moisture percentage
    readings[i].resistance = long(float(RESISTOR_VALUE) * (supplyVoltage - sensorVoltage) / sensorVoltage + 0.5);
    readings[i].moisture = min(int(pow(readings[i].resistance / 331.55, 1.0 / -1.695) * 100 + 0.5), 100);
  }

  // Sort readings to get the median value
  sortReadings();

  // Update LCD with the median moisture value
  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(readings[NUM_READS / 2].moisture);
  lcd.print("%");

  // Delay before the next measurement
  delay(3000);
}

// Function to serve the HTML page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Soil Moisture</title></head><body>";
  html += "<h1>Soil Moisture Sensor</h1>";
  html += "<p>Moisture: " + String(readings[NUM_READS / 2].moisture) + "%</p>";
  html += "<p>Resistance: " + String(readings[NUM_READS / 2].resistance) + " Ohms</p>";
  html += "<p><a href='/'><button>Refresh</button></a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Helper function to switch between pins for current paths
void switchCurrentPath() {
  if (activePin == 18) {
    activePin = 19;
    supplyPin = 34;  // ESP32 analog pin for supply voltage
    sensorPin = 35;  // ESP32 analog pin for sensor voltage
  } else {
    activePin = 18;
    supplyPin = 35;
    sensorPin = 34;
  }
}

// Helper function to sort moisture values using selection sort
void sortReadings() {
  MoistureData temp;
  for (int i = 0; i < NUM_READS - 1; i++) {
    for (int j = i + 1; j < NUM_READS; j++) {
      if (readings[i].moisture > readings[j].moisture) {
        temp = readings[i];
        readings[i] = readings[j];
        readings[j] = temp;
      }
    }
  }
}

