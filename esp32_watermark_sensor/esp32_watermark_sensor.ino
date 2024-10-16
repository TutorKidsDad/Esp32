#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Include the library for ESP32 soil moisture sensor
#define NUM_READS 11    // Number of sensor reads for filtering

typedef struct {        // Structure to store filtered moisture percentage and resistance values
  int moisture;
  long resistance;
} values;

// Constants
const long knownResistor = 4700;  // Resistor value in Ohms
int activeDigitalPin = 18;        // Pins for switching current paths
int supplyVoltageAnalogPin;       // Pins for reading supply voltage
int sensorVoltageAnalogPin;       // Pins for reading sensor voltage
int supplyVoltage;                // Supply voltage value
int sensorVoltage;                // Sensor voltage value
values valueOf[NUM_READS];        // Stores readings for filtering

// LCD initialization (I2C address 0x27 for 16x2 display)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

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
  pinMode(18, OUTPUT);  // Digital pin 18 for current path 1
  pinMode(19, OUTPUT);  // Digital pin 19 for current path 2
  delay(200);
}

void loop() {
  // Reading sensor values, filtering, and calculating resistance
  for (int i = 0; i < NUM_READS; i++) {
    setupCurrentPath();  // Configure pins for measurement

    // Turn on voltage supply for the sensor
    digitalWrite(activeDigitalPin, HIGH);
    delay(25);
    supplyVoltage = analogRead(supplyVoltageAnalogPin);  // Read supply voltage
    sensorVoltage = analogRead(sensorVoltageAnalogPin);  // Read sensor voltage
    digitalWrite(activeDigitalPin, LOW);  // Turn off voltage supply
    delay(25);

    // Calculate resistance and moisture percentage
    valueOf[i].resistance = long(float(knownResistor) * (supplyVoltage - sensorVoltage) / sensorVoltage + 0.5);
    valueOf[i].moisture = min(int(pow(valueOf[i].resistance / 331.55, 1.0 / -1.695) * 100 + 0.5), 100);
  }

  // Sort and display the median moisture value
  sortMoistures();
  
  // Print values to the Serial Monitor and LCD
  Serial.print("Resistance: ");
  Serial.print(valueOf[NUM_READS / 2].resistance);
  Serial.print(", Moisture: ");
  Serial.println(valueOf[NUM_READS / 2].moisture);

  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(valueOf[NUM_READS / 2].moisture);
  lcd.print("%");

  // Delay before the next measurement
  delay(3000);
}

// Helper function to switch between pins for current paths
void setupCurrentPath() {
  if (activeDigitalPin == 18) {
    activeDigitalPin = 19;
    supplyVoltageAnalogPin = 34;  // ESP32 analog pin for supply voltage
    sensorVoltageAnalogPin = 35;  // ESP32 analog pin for sensor voltage
  } else {
    activeDigitalPin = 18;
    supplyVoltageAnalogPin = 35;
    sensorVoltageAnalogPin = 34;
  }
}

// Helper function to sort moisture values using selection sort
void sortMoistures() {
  int j;
  values temp;
  for (int i = 0; i < NUM_READS - 1; i++) {
    for (j = i + 1; j < NUM_READS; j++) {
      if (valueOf[i].moisture > valueOf[j].moisture) {
        temp = valueOf[i];
        valueOf[i] = valueOf[j];
        valueOf[j] = temp;
      }
    }
  }
}
