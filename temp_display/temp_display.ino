#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <Wire.h>

// Define platform-specific pins
#ifdef ESP32
  #define ONE_WIRE_BUS 32  // Use GPIO 32 for OneWire on ESP32
  #define SDA_PIN 21       // Use GPIO 21 for SDA on ESP32
  #define SCL_PIN 22       // Use GPIO 22 for SCL on ESP32
#elif defined(ESP8266)
  #define ONE_WIRE_BUS D2  // Use D2 for OneWire on ESP8266
  #define SDA_PIN D1        // Use D1 for SDA on ESP8266
  #define SCL_PIN D2        // Use D2 for SCL on ESP8266 (shared with OneWire)
#else
  #define ONE_WIRE_BUS A3  // Use A3 for OneWire on Arduino Uno
  #define SDA_PIN A4        // Use A4 for SDA on Arduino Uno
  #define SCL_PIN A5        // Use A5 for SCL on Arduino Uno
#endif

#define TEMP_SIZE 7

// Setup a OneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our OneWire reference to Dallas Temperature.
DallasTemperature DS18B20(&oneWire);

char temperatureCString[TEMP_SIZE];
char previousTemp[TEMP_SIZE] = "init";

LiquidCrystal_I2C lcd(0x27, 16, 2); // The I2C address may be different in your project, see the notes below.

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd.print("HELLO");
  lcd.setCursor(5, 1);
  lcd.print("WORLD");

  delay(2000);

  DS18B20.begin();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp.:");
}

void getTemperature() {
  float tempC;
  do {
    DS18B20.requestTemperatures();
    tempC = DS18B20.getTempCByIndex(0);
    dtostrf(tempC, 3, 1, temperatureCString);
    if (tempC == (-127)) {
      delay(100);
    }
  } while (tempC == (-127.0));
}

void loop() {
  getTemperature();

  Serial.println(temperatureCString);

  if (strcmp(previousTemp, temperatureCString) != 0) {
    lcd.setCursor(7, 0);
    lcd.print(temperatureCString);
    strncpy(previousTemp, temperatureCString, TEMP_SIZE);
  }

  delay(2000);
}
