#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Library for I2C LCD
#include <WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 for a 16 chars and 2 line display

WiFiClient client;  // Declare the WiFi client object

// WiFi settings
const char* ssid = "testing";
const char* password = "password";

// ThingSpeak settings
unsigned long channelID = 617957;
const char* apiKey = "";

// Sensor settings
const int sensor_pin = A0;  // Soil moisture analog sensor pin
const int AirValue = 3400;   // Replace with actual air value
const int WaterValue = 1300; // Replace with actual water value
int soilMoistureValue = 0;
int soilmoisturepercent = 0;

// DHT11 settings
#define DHTPIN 2  // Pin where the DHT11 is connected
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// DS18B20 settings
#define ONE_WIRE_BUS 4  // Pin where the DS18B20 is connected
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Time to sleep (in microseconds) -> 10 minutes = 600 seconds
const uint64_t SLEEP_DURATION = 600000000; // 600 seconds = 10 minutes

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    lcd.setCursor(0, 0);
    lcd.print("Connecting...");
  }
  Serial.println("Connected to WiFi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  // Initialize sensors and ThingSpeak
  ThingSpeak.begin(client);
  dht.begin();
  sensors.begin();

  delay(1000);
}

void loop() {
  // Read soil moisture sensor value
  soilMoistureValue = analogRead(sensor_pin);
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.print("Soil Moisture (in %): ");
  Serial.print(soilmoisturepercent);
  Serial.println("%");

  // Read temperature and humidity from DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read temperature from DS18B20
  sensors.requestTemperatures();
  float ds18b20Temperature = sensors.getTempCByIndex(0);

  // Check for failed sensor readings
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor");
    lcd.setCursor(0, 1);
    lcd.print("DHT11 Failed!");
    delay(2000);
    return;
  }

  // Update ThingSpeak fields
  ThingSpeak.setField(1, soilmoisturepercent);
  ThingSpeak.setField(2, temperature);
  ThingSpeak.setField(3, humidity);
  ThingSpeak.setField(4, ds18b20Temperature);

  // Display data on the I2C LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  lcd.print(soilmoisturepercent);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp:");
  lcd.print(temperature);
  lcd.print("C Hum:");
  lcd.print(humidity);
  lcd.print("%");

  // Write to ThingSpeak
  int response = ThingSpeak.writeFields(channelID, apiKey);
  if (response == 200) {
    Serial.println("Data sent to ThingSpeak successfully");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Data Sent");
  } else {
    Serial.println("Error sending data to ThingSpeak");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Send Error");
  }

  // Display DS18B20 temperature
  lcd.setCursor(0, 1);
  lcd.print("DS18B20: ");
  lcd.print(ds18b20Temperature);
  lcd.print("C");

  delay(5000);  // Display data for 5 seconds before sleep

  // Enter deep sleep for 10 minutes
  Serial.println("Entering deep sleep for 10 minutes...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sleeping...");
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION);
  esp_deep_sleep_start();
}
