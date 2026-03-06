#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define DATA_PIN 23
#define CLK_PIN 18
#define CS_PIN 5

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

const char* ssid = "testing";
const char* password = "password";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST offset

char timeString[10];
bool colon = true;

unsigned long lastScroll = 0;

void setup()
{

  Serial.begin(115200);

  display.begin();
  display.setIntensity(3);
  display.displayClear();

  display.displayText("CONNECT WIFI", PA_CENTER, 100, 0, PA_SCROLL_LEFT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  display.displayClear();
  display.displayText("WIFI OK", PA_CENTER, 100, 1000, PA_SCROLL_LEFT);

  timeClient.begin();
}

void loop()
{

  timeClient.update();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();

  colon = !colon;

  if (colon)
    sprintf(timeString, "%02d:%02d", hours, minutes);
  else
    sprintf(timeString, "%02d %02d", hours, minutes);

  display.displayClear();
  display.print(timeString);

  if (millis() - lastScroll > 20000)
  {

    String date = timeClient.getFormattedTime();

    display.displayClear();
    display.displayText("HAVE A NICE DAY", PA_CENTER, 80, 3000, PA_SCROLL_LEFT);

    lastScroll = millis();
  }

  delay(1000);
}
