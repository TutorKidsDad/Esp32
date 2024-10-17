/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Libraries for SD card
#include "FS.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

// DS18B20 libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>

// Define deep sleep options
uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for microseconds to seconds
uint64_t TIME_TO_SLEEP = 600;       // Sleep for 10 minutes = 600 seconds

// Data logging and SD card constants
#define LOG_INTERVAL 20000   // Log every 20 seconds
#define SYNC_INTERVAL 20000  // Sync every 20 seconds
uint32_t syncTime = 0;       // Time of last sync()

RTC_DS3231 RTC;  // Real Time Clock object
File logfile;

// Replace with your network credentials
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Define CS pin for the SD card module
#define SD_CS 5

// Save reading number in RTC memory
RTC_DATA_ATTR int readingID = 0;

// Data wire for DS18B20 is connected to ESP32 GPIO 21
#define ONE_WIRE_BUS 21
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Temperature sensor variables
float temperature;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;

void error(char *str) {
  Serial.print("Error: ");
  Serial.println(str);
  while(1);
}

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");

  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    error("Card failed, or not present");
  }

  // Create a new file
  char filename[] = "LOG__000.CSV";
  for (uint8_t i = 0; i < 101; i++) {
    filename[5] = i / 10 + '0';
    filename[6] = i % 10 + '0';
    if (!SD.exists(filename)) {
      logfile = SD.open(filename, FILE_WRITE);
      break;
    }
  }

  if (!logfile) {
    error("Couldn't create file");
  }

  // Initialize RTC
  Wire.begin();
  if (!RTC.begin()) {
    logfile.println("RTC failed");
  }

  // Log file header
  logfile.println("CO2 Datalogger");
  logfile.print("File Name: ");
  logfile.println(filename);
  logfile.println("MilliSec,Stamp,Date,Time,CO2_1,CO2_2,CO2_3,Temperature_1(C),Humidity_1,Temperature_2(C),Humidity_2,Temperature_3(C),Humidity_3");

  // Start deep sleep
  Serial.println("DONE! Going to sleep now.");
  esp_deep_sleep_start();
}

void loop() {
  // The ESP32 will be in deep sleep and never reach the loop
}

// Function to get temperature
void getReadings() {
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temperature);
}

// Write sensor readings to the SD card
void logSDCard() {
  String dataMessage = String(readingID) + "," + String(dayStamp) + "," + String(timeStamp) + "," + 
                String(temperature) + "\r\n";
  Serial.print("Save data: ");
  Serial.println(dataMessage);
  appendFile(SD, "/data.txt", dataMessage.c_str());
}

// Write to SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to SD card
void appendFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}
