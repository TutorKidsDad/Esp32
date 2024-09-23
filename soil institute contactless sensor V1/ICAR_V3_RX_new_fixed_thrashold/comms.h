#ifndef COMMS_H
#define COMMS_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

//TO READ THE SENSOR VALUES
#define THINGSPEAK_API_KEY1 "RWZ07BL7XBPQBS01" // Replace with your ThingSpeak read API key
#define THINGSPEAK_CHANNEL_ID1 "2614146" // Replace with your ThingSpeak channel ID
#define THINGSPEAK_API_KEY2 "Q2E5V1PCJ325HLVO" // Replace with your ThingSpeak read API key
#define THINGSPEAK_CHANNEL_ID2 "2615653" // Replace with your ThingSpeak channel ID
#define THINGSPEAK_READ_URL "https://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds/last.json?api_key=YOUR_READ_API_KEY"

// TO UPDATE THE VALVE AND PUNP STATUS
#define THINGSPEAK_API_KEY3 "IGM994854IVCVD25" // Replace with your ThingSpeak WRITE API key
#define THINGSPEAK_CHANNEL_ID3 "2615656" // Replace with your ThingSpeak channel ID TO UPDATE STATUS
//#define THINGSPEAK_WRITE_URL "https://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds/last.json?api_key=YOUR_WRITE_API_KEY"
#define THINGSPEAK_WRITE_URL "http://api.thingspeak.com/update?api_key=" THINGSPEAK_API_KEY3


#define WIFI_SSID "swapnil2@4"
#define WIFI_PASS "TPS@462016"

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

int ts0, ts1, ts2, ts3;

void connect_wifi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected!");
}
/*
void fetch_sensor_data()
{
  HTTPClient http;
  
  // Construct the URLs for fetching data from ThingSpeak
  String url0 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID1) + "/fields/1.json?api_key=" + String(THINGSPEAK_API_KEY1) + "&results=1";
  String url1 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID2) + "/fields/1.json?api_key=" + String(THINGSPEAK_API_KEY2) + "&results=1";
  //String url2 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID) + "/fields/3.json?api_key=" + String(THINGSPEAK_API_KEY) + "&results=1";
 // String url3 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID) + "/fields/4.json?api_key=" + String(THINGSPEAK_API_KEY) + "&results=1";
  
  // Fetch data from ThingSpeak
  http.begin(url0);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Parse JSON payload to extract data
    // Update wp0, humidity0, temp0, vbatt0, moisture0 here
    // Example: wp0 = parse_json(payload, "field1"); // Implement parse_json as needed
    Serial.println("WP0: " + String(wp0));
  }
  http.end();
  
  http.begin(url1);
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Parse JSON payload to extract data
    // Update wp1, humidity1, temp1, vbatt1, moisture1 here
    Serial.println("WP1: " + String(wp1));
  }
  http.end();
/*  
  http.begin(url2);
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Parse JSON payload to extract data
    // Update wp2, humidity2, temp2, vbatt2, moisture2 here
    Serial.println("WP2: " + String(wp2));
  }
  http.end();
  
  http.begin(url3);
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    // Parse JSON payload to extract data
    // Update wp3, humidity3, temp3, vbatt3, moisture3 here
    Serial.println("WP3: " + String(wp3));
  }
  http.end();
  
//}
*/
void fetch_sensor_data()
{
  HTTPClient http;
  
  // Construct the URLs for fetching data from ThingSpeak
  String url0 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID1) + "/fields/1.json?api_key=" + String(THINGSPEAK_API_KEY1) + "&results=1";
  String url1 = "http://api.thingspeak.com/channels/" + String(THINGSPEAK_CHANNEL_ID2) + "/fields/1.json?api_key=" + String(THINGSPEAK_API_KEY2) + "&results=1";
  
  // Fetch data from ThingSpeak
  http.begin(url0);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    
    // Parse JSON payload
    DynamicJsonDocument doc(1024); // Adjust size as needed
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
  
  http.begin(url1);
  httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    
    // Parse JSON payload
    DynamicJsonDocument doc(1024); // Adjust size as needed
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
}



#endif
