//board : doit esp32 devkit v1

//#define DEV_ID 0
#define DEV_NAME "ICAR-RX"

#include "pins.h"
#include "comms.h"
#include "OTA.h"
#include "display.h"
#include "sensors.h"
#include <esp_sleep.h> // Include the ESP32 sleep library
#include <time.h>      // Include time library // CHANGE: Added time library

int prev_t, uvc=0;

void setup() 
{
  Serial.begin(BAUD_RATE);
  Serial.println("RX Init");
  init_pins();
  setup_OTA();
  connect_wifi();
  init_display();
  prev_t = millis();
  psu_swap_t = millis();

    // Synchronize time using SNTP
// Synchronize time using SNTP
configTime(0, 0, "pool.ntp.org", "time.nist.gov"); // Change time zone as needed
delay(5000);
Serial.println("Waiting for time");
while(!time(nullptr)) {
  delay(1000);
  Serial.print(".");
}
Serial.println("\nTime synchronized");

// Print the current time
time_t now = time(nullptr); // Get current time
struct tm* timeinfo = localtime(&now); // Convert to local time
char timeString[20]; // Buffer to hold formatted time

// Format the time as YYYY-MM-DD HH:MM:SS
strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeinfo);

Serial.println("Current time: " + String(timeString));

}

void loop() 
{
  ArduinoOTA.handle();
  //update_screen();
  read_voltage();
  if(millis() - prev_t > 5000)
  {
    prev_t = millis();
    //switch_screen();
  }
 // check_server();
  readThingSpeak();
  fetch_sensor_data();
  switch_valves();
  check_pump();
  
  
// uncomment to re enable battery protection 

  if(v_batt < 11.63 && v_psu < 10.0)
  {
    uvc++;
    if(uvc > 200)
    {
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("Battery low"+String(v_batt));
      //display.display();
      turn_relay_off();
      while(1);
    }
    delay(100);
  }
  else
    uvc=0;
// Enter deep sleep for 3 minutes (180000 milliseconds)
  esp_sleep_enable_timer_wakeup(180000000); // Time in microseconds
  Serial.println("Going to deep sleep for 3 minutes");
  esp_deep_sleep_start();
  delay(1000);
}


void readThingSpeak() 
{
  HTTPClient http;
  
  // Construct URLs
  String url1 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID1) + "/feeds/last.json?api_key=" + String(THINGSPEAK_API_KEY1);
  String url2 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID2) + "/feeds/last.json?api_key=" + String(THINGSPEAK_API_KEY2);
  
  // Print the URLs to the Serial Monitor
  Serial.println("URL1: " + url1);
  Serial.println("URL2: " + url2);

  // Get the current time once
  time_t currentTime = time(nullptr);

  // Check URL1
  http.begin(url1);
  int httpCode1 = http.GET();
  if (httpCode1 > 0) 
  {
    String payload1 = http.getString();
    Serial.println("Payload for URL1: " + payload1); // Print the data to Serial Monitor

    DynamicJsonDocument doc1(1024);
    DeserializationError error1 = deserializeJson(doc1, payload1);
    if (!error1) 
    {
      const char* created_at1 = doc1["created_at"];
      time_t dataTime1 = parse_thingspeak_time(created_at1);
      double timeDifference1 = difftime(currentTime, dataTime1);

      if (timeDifference1 > 600) // 600 seconds = 10 minutes
      {
        Serial.println("Data from URL1 is too old.");
        //esp_deep_sleep_start(); // Enter deep sleep mode
      }
    } 
    else 
    {
      Serial.println("Failed to parse JSON for URL1");
    }
  } 
  else 
  {
    Serial.println("Error on HTTP request for URL1");
  }
  http.end();

  // Check URL2
  http.begin(url2);
  int httpCode2 = http.GET();
  if (httpCode2 > 0) 
  {
    String payload2 = http.getString();
    Serial.println("Payload for URL2: " + payload2); // Print the data to Serial Monitor

    DynamicJsonDocument doc2(1024);
    DeserializationError error2 = deserializeJson(doc2, payload2);
    if (!error2) 
    {
      const char* created_at2 = doc2["created_at"];
      time_t dataTime2 = parse_thingspeak_time(created_at2);
      double timeDifference2 = difftime(currentTime, dataTime2);

      if (timeDifference2 > 600) // 600 seconds = 10 minutes
      {
        Serial.println("Data from URL2 is too old.");
       // esp_deep_sleep_start(); // Enter deep sleep mode
      }
    } 
    else 
    {
      Serial.println("Failed to parse JSON for URL2");
    }
  } 
  else 
  {
    Serial.println("Error on HTTP request for URL2");
  }
  http.end();
}


time_t parse_thingspeak_time(const char* timeString) 
{
  struct tm tm;
  strptime(timeString, "%Y-%m-%dT%H:%M:%SZ", &tm);
  return mktime(&tm);
}
/*
void readThingSpeak() {
  HTTPClient http;
  http.begin(THINGSPEAK_READ_URL);
  int httpCode = http.GET();
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("payload is "+ payload); // Print the data to Serial Monitor
  } else {
    Serial.println("Error on HTTP request");
  }
  http.end();
}
*/
