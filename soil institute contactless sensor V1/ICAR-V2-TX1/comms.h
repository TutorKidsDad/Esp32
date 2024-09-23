#ifndef COMMS_H
#define COMMS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_sleep.h> // Include the ESP32 sleep library

#include <LiquidCrystal_I2C.h>

#define BAUD_RATE 115200
#define WIFI_SSID "testing"
#define WIFI_PASS "password"

// LCD dimensions
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

// Create an LCD object with I2C address 0x27 (adjust if necessary)
LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);


// ThingSpeak settings
#define THINGSPEAK_API_KEY "EPAF925KG5ROLXA2" // Replace with your ThingSpeak WRITE API key
#define THINGSPEAK_SERVER "http://api.thingspeak.com"

// Define these at the top of your file or in an appropriate header file
float wp=0;          // Example variable for some measurement
float humidity=0;    // Humidity value
float temp=0;        // Temperature value
float moisture=0;    // Soil moisture value
float vbatt=0;       // Battery voltage
const unsigned long connectTimeout = 30000; // 30 seconds timeout
const unsigned long sleepDuration = 180000000; // 3 minutes in milliseconds


// Device settings
#ifdef DEV_ID0
  #define DEV_ID 0
  #define DEV_NAME "Transmitter-TX0"
#endif

#ifdef DEV_ID1
  #define DEV_ID 1
  #define DEV_NAME "Transmitter-TX1"
#endif

#ifdef DEV_ID2
  #define DEV_ID 2
  #define DEV_NAME "MEGH-TX2"
#endif

#ifdef DEV_ID3
  #define DEV_ID 3
  #define DEV_NAME "MEGH-TX3"
#endif

WiFiClient client;
/*
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
    // Start with one pin high and the other low
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, HIGH);
}
*/
void connectToWiFi() {
    lcd.init(); // Initialize the LCD
lcd.backlight(); // Turn on the backlight
  Serial.println("Connecting to WiFi...");
   lcd.clear();  // Clear the LCD
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi...");  // Display connection status
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  unsigned long startTime = millis();
  bool connected = false;

  while (millis() - startTime < connectTimeout) {
    if (WiFi.status() == WL_CONNECTED) {
      connected = true;
      break;
    }
    delay(500); // Delay to avoid spamming the status check
    Serial.print(".");
  }

  if (connected) {
    Serial.println("\nConnected to WiFi");
         lcd.setCursor(0, 3);
  lcd.print("Connected to WiFi..");  // Display connection status
            delay(3000);
    // Start with one pin high and the other low
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, HIGH);
  } else {
    Serial.println("\nFailed to connect to WiFi, going to sleep...");
      lcd.setCursor(0, 2);
  lcd.print("Failed to connect..");  // Display connection status
    // Put ESP32 into deep sleep
    esp_sleep_enable_timer_wakeup(sleepDuration);
         lcd.setCursor(0, 3);
     lcd.print("Going to deep sleep..");  // Display connection status
          delay(5000);
     lcd.noBacklight(); // Turn off the backlight
    delay(3000);
    esp_deep_sleep_start();
    delay(3000);
  }
}


void upload_data()
{
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
    delay(1000);
  }

  HTTPClient http;
  String url = THINGSPEAK_SERVER;
  url += "/update?api_key=" + String(THINGSPEAK_API_KEY);
  url += "&field1=" + String(wp);
  url += "&field2=" + String(humidity);
  url += "&field3=" + String(temp);
  url += "&field4=" + String(moisture);
  url += "&field5=" + String(vbatt);

  http.begin(client, url);
  int httpCode = http.GET();
  if (httpCode > 0) {
    Serial.println("Data uploaded to ThingSpeak");
    Serial.println(url);
  } else {
    Serial.println("Upload Failed!");
  }
  http.end();

  digitalWrite(BUZZER, HIGH);
  delay(1000);
  digitalWrite(BUZZER, LOW);
}

#endif
