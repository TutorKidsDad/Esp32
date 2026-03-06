#include <WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <time.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN 23
#define CS_PIN 5
#define CLK_PIN 18

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const char* ssid = "testing";
const char* password = "password";

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;   // IST (5.5 hours)
const int daylightOffset_sec = 0;

void setup() {

  Serial.begin(115200);

  display.begin();
  display.setIntensity(5);
  display.displayClear();

  WiFi.begin(ssid, password);

  display.print("WIFI");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  display.print("SYNC");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void loop() {

  struct tm timeinfo;

  if(!getLocalTime(&timeinfo)){
    display.print("ERR");
    return;
  }

  char timeStr[6];
  sprintf(timeStr,"%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min);

  display.print(timeStr);

  delay(1000);
}
