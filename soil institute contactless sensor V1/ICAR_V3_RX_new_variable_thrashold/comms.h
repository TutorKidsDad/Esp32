#ifndef COMMS_H
#define COMMS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <esp_sleep.h> // Include the ESP32 sleep library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD dimensions
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

// Create an LCD object with I2C address 0x27 (adjust if necessary)
LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

//TO READ THE SENSOR VALUES
#define THINGSPEAK_API_KEY1 "RWZ07BL7XBPQBS01" // Replace with your ThingSpeak read API key
#define THINGSPEAK_CHANNEL_ID1 "2614146" // Replace with your ThingSpeak channel ID
#define THINGSPEAK_API_KEY2 "Q2E5V1PCJ325HLVO" // Replace with your ThingSpeak read API key
#define THINGSPEAK_CHANNEL_ID2 "2615653" // Replace with your ThingSpeak channel ID
#define THINGSPEAK_READ_URL "https://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds/last.json?api_key=YOUR_READ_API_KEY"

// TO UPDATE THE VALVE AND PUMP STATUS
#define THINGSPEAK_API_KEY3 "IGM994854IVCVD25" // Replace with your ThingSpeak WRITE API key
#define THINGSPEAK_CHANNEL_ID3 "2615656" // Replace with your ThingSpeak channel ID TO UPDATE STATUS
//#define THINGSPEAK_WRITE_URL "https://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds/last.json?api_key=YOUR_WRITE_API_KEY"
#define THINGSPEAK_WRITE_URL "http://api.thingspeak.com/update?api_key=" THINGSPEAK_API_KEY3

// TO FETCH VALUES FROM CHANNEL 4 OF THRASHOLD
#define THINGSPEAK_CHANNEL_ID "2627097"
#define THINGSPEAK_API_KEY "MJ4EF43L5F3A783L"


#define WIFI_SSID "testing"
#define WIFI_PASS "password"

#define BAUD_RATE 115200
#define TIMEOUT 60000
//#define DATA_TIMEOUT 300000 // 5 minutes in milliseconds
// Function prototypes
void fetch_sensor_data();
bool check_server();


float wp0 = 0.0, humidity0 = 0.0, temp0 = 0.0, vbatt0 = 0.0, moisture0 = 0.0;
float wp1 = 0.0, humidity1 = 0.0, temp1 = 0.0, vbatt1 = 0.0, moisture1 = 0.0;
float wp2 = 0.0, humidity2 = 0.0, temp2 = 0.0, vbatt2 = 0.0, moisture2 = 0.0;
float wp3 = 0.0, humidity3 = 0.0, temp3 = 0.0, vbatt3 = 0.0, moisture3 = 0.0;

// Variables to hold field values
float field1Value = 0.0;
float field2Value = 0.0;

int ts0, ts1, ts2, ts3;

const unsigned long connectTimeout = 30000; // 30 seconds timeout
const unsigned long sleepDuration = 180000000; // 3 minutes in milliseconds

/*
void connect_wifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
        // Start with one pin high and the other low
    digitalWrite(PIN1, HIGH);
    digitalWrite(PIN2, LOW);
  }
  Serial.println("Connected!");
      // Start with one pin high and the other low
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, HIGH);
}
*/
void connectToWiFi() {
  lcd.init(); // Initialize the LCD
lcd.backlight(); // Turn on the backlight
  //   lcd.noBacklight(); // Turn off the backlight
  Serial.println("Connecting to WiFi..");
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
 // lcd.clear();  // Clear the LCD
  lcd.setCursor(0, 2);
  lcd.print("Failed to connect..");  // Display connection status
//  delay(5000); // Pause to let the user see the initial message
    // Put ESP32 into deep sleep
   esp_sleep_enable_timer_wakeup(sleepDuration);
     lcd.setCursor(0, 3);
     lcd.print("Going to deep sleep..");  // Display connection status
          delay(5000);
     lcd.noBacklight(); // Turn off the backlight
    esp_deep_sleep_start();
     delay(3000);
  }
}

void fetch_sensor_data()
{
  HTTPClient http;
  
  // Construct the URLs for fetching WP data from ThingSpeak
  String url0 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID1) + "/fields/1.json?api_key=" + String(THINGSPEAK_API_KEY1) + "&results=1";
  String url1 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID2) + "/fields/1.json?api_key=" + String(THINGSPEAK_API_KEY2) + "&results=1";
 
 
 // Construct the URL for the GET request for THREASHOLD
  String url = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID) + "/feeds/1.json?api_key=" + String(THINGSPEAK_API_KEY);
  Serial.println("Fetching data from URL: " + url);

  // Fetch data from ThingSpeak
  http.begin(url0);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Raw JSON payload:");
Serial.println(payload);
    // Parse JSON payload
    DynamicJsonDocument doc(2048); // Adjust size as needed
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      // Extract the value from field1
      wp0 = doc["feeds"][0]["field1"].as<float>(); // Assuming field1 contains a float value

      Serial.println("WP0: " + String(wp0));  

    } else {
      Serial.println("Failed to parse JSON for WP0");
    }
  } else {
    Serial.println("Failed to fetch WP0 data");
  }
  http.end();
  delay(1000);

  http.begin(url1);
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    Serial.println("Raw JSON payload:");
Serial.println(payload);
    // Parse JSON payload
    DynamicJsonDocument doc(2048); // Adjust size as needed
    DeserializationError error = deserializeJson(doc, payload);
    
    if (!error) {
      // Extract the value from field1
      wp1 = doc["feeds"][0]["field1"].as<float>(); // Assuming field1 contains a float value
      Serial.println("WP1: " + String(wp1));
    } else {
      Serial.println("Failed to parse JSON for WP1");
    }
  } else {
    Serial.println("Failed to fetch WP1 data");
  }
  http.end();
delay(1000);


/*
// Fetch thrashold data from ThingSpeak
  // Start the HTTP request
  http.begin(url);
  // Check the returning code
  if (httpCode > 0) {
    // HTTP header has been sent and server response header has been handled
    Serial.printf("HTTP GET request code: %d\n", httpCode);

    // File found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Received payload:");
      Serial.println(payload);

      // Parse JSON payload
      DynamicJsonDocument doc(2048); // Adjust size as needed
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        // Extract the value from field1 and field2
        field1Value = doc["field1"].as<float>();
        field2Value = doc["field2"].as<float>();

        Serial.println("Field1: " + String(field1Value));
        Serial.println("Field2: " + String(field2Value));
      } else {
        Serial.print("Failed to parse JSON, error: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.println("Failed to fetch data");
    }
  } else {
    Serial.printf("Failed to make request, error: %s\n", http.errorToString(httpCode).c_str());
  }

  // Close the connection
  http.end();
  ////////////////////////////////////////////////
  */
delay(1000);
}

// Function to fetch data from ThingSpeak
void fetchDataFromThingSpeak() {
  HTTPClient http;

  // Construct the URL for the GET request
  String url = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID) + "/feeds/last.json?api_key=" + String(THINGSPEAK_API_KEY);
  Serial.println("Fetching data from URL: " + url);

  // Start the HTTP request
  http.begin(url);
  int httpCode = http.GET();

  // Check the returning code
  if (httpCode > 0) {
    // HTTP header has been sent and server response header has been handled
    Serial.printf("HTTP GET request code: %d\n", httpCode);

    // File found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Received payload:");
      Serial.println(payload);

      // Parse JSON payload
      DynamicJsonDocument doc(1024); // Adjust size as needed
      DeserializationError error = deserializeJson(doc, payload);

      if (!error) {
        // Extract the value from field1 and field2
        field1Value = doc["field1"].as<float>();
        field2Value = doc["field2"].as<float>();

        Serial.println("Field1: " + String(field1Value));
        Serial.println("Field2: " + String(field2Value));
      } else {
        Serial.print("Failed to parse JSON, error: ");
        Serial.println(error.c_str());
      }
    } else {
      Serial.println("Failed to fetch data");
    }
  } else {
    Serial.printf("Failed to make request, error: %s\n", http.errorToString(httpCode).c_str());
  }

  // Close the connection
  http.end();
}
#endif
