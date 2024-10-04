#include "FS.h"
#include "SD.h"
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include "time.h"

// Replace with your network credentials
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Timer variables
unsigned long lastTime = 0;
const unsigned long timerDelay = 30000; // 30 seconds

// BME280 I2C
Adafruit_BME280 bme;

// NTP server and epoch time variable
const char* ntpServer = "pool.ntp.org";
unsigned long epochTime; 

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

// Init BME280
void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

// Initialize SD card
void initSDCard(){
  if (!SD.begin()) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.printf("SD Card Type: %s\n", (cardType == CARD_MMC) ? "MMC" : (cardType == CARD_SD) ? "SDSC" : (cardType == CARD_SDHC) ? "SDHC" : "UNKNOWN");
  Serial.printf("SD Card Size: %lluMB\n", SD.cardSize() / (1024 * 1024));
}

// Write to the SD card
void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_WRITE);
  if (file) {
    if (file.print(message)) {
      Serial.println("File written");
    } else {
      Serial.println("Write failed");
    }
    file.close();
  } else {
    Serial.println("Failed to open file for writing");
  }
}

// Append data to the SD card
void appendFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, FILE_APPEND);
  if (file) {
    if (file.print(message)) {
      Serial.println("Message appended");
    } else {
      Serial.println("Append failed");
    }
    file.close();
  } else {
    Serial.println("Failed to open file for appending");
  }
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return(0);
  }
  time(&now);
  return now;
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initBME();
  initSDCard();
  configTime(0, 0, ntpServer);

  // Create data file if it doesn't exist
  if (!SD.open("/data.txt")) {
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Epoch Time, Temperature, Humidity, Pressure \r\n");
  } else {
    Serial.println("File already exists");  
  }
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Get epoch time and sensor readings
    epochTime = getTime();
    float temp = bme.readTemperature();
    float hum = bme.readHumidity();
    float pres = bme.readPressure() / 100.0F;

    // Concatenate data
    String dataMessage = String(epochTime) + "," + String(temp) + "," + String(hum) + "," + String(pres) + "\r\n";
    Serial.print("Saving data: ");
    Serial.println(dataMessage);

    // Append data to file
    appendFile(SD, "/data.txt", dataMessage.c_str());
    lastTime = millis();
  }
}
