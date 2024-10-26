#include "SPIFFS.h"
#include <JPEGDecoder.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFiManager.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#define DHTPIN 19    
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128
#define SCLK_PIN 18
#define MOSI_PIN 23
#define DC_PIN 17
#define CS_PIN 5
#define RST_PIN 4

// Color definitions
#define BLACK 0x0000
#define WHITE 0xFFFF

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);

WebServer server(80);
int prev_temp;
int prev_humidity;

void setup() {
    Serial.begin(115200);
    
    tft.begin();
    tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    dht.begin();

    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);

    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;

    if (!SPIFFS.begin()) {
        while (1) yield();
    }

    initScreen();
    
    // Setup WiFiManager
    WiFiManager wifiManager;
    wifiManager.autoConnect("DHT_Sensor");

    // Start mDNS service
    if (!MDNS.begin("dhtsensor")) {
        Serial.println("Error starting mDNS");
        return;
    }
    Serial.println("mDNS started. You can access the device at http://dhtsensor.local");

    // Set up server routes
    server.on("/", HTTP_GET, handleRoot);
    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    // Handle client requests
    server.handleClient();
    
    // Delay between measurements.
    delay(delayMS);
    
    // Get temperature and humidity events.
    updateSensorData();
}

void initScreen() {
    // Initial screen setup (optional)
}

void updateSensorData() {
    sensors_event_t event;
    
    dht.temperature().getEvent(&event);
    if (!isnan(event.temperature)) {
        updateTemp((int)event.temperature);
    }
    
    dht.humidity().getEvent(&event);
    if (!isnan(event.relative_humidity)) {
        updateHumidity((int)event.relative_humidity);
    }
}

void updateTemp(int temp) {
    if (prev_temp != temp) {
        tft.fillRect(64, 0, 128, 64, WHITE);
        tft.setCursor(70, 24);
        tft.setTextColor(BLACK);
        tft.setTextSize(3);
        String tempString = String(temp) + "C";
        tft.print(tempString);
        prev_temp = temp;
    }
}

void updateHumidity(int humidity) {
    if (prev_humidity != humidity) {
        tft.fillRect(64, 64, 128, 64, WHITE);
        tft.setCursor(70, 88);
        tft.setTextColor(BLACK);
        tft.setTextSize(3);
        String humidityString = String(humidity) + "%";
        tft.print(humidityString);
        prev_humidity = humidity;
    }
}

// Web server route handler
void handleRoot() {
    String html = "<html><body><h1>DHT Sensor Readings</h1>";
    html += "<p>Temperature: " + String(prev_temp) + " °C</p>";
    html += "<p>Humidity: " + String(prev_humidity) + " %</p>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}
