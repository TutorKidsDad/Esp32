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
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);

unsigned long lastScroll = 0;
bool scrollMode = false;

String days[]={
"Sunday","Monday","Tuesday","Wednesday",
"Thursday","Friday","Saturday"
};

void setup()
{
Serial.begin(115200);

display.begin();
display.setIntensity(4);
display.displayClear();

WiFi.begin(ssid,password);

while(WiFi.status()!=WL_CONNECTED)
{
delay(500);
Serial.print(".");
}

Serial.println("Connected");

timeClient.begin();
}

void loop()
{

timeClient.update();

int h = timeClient.getHours();
int m = timeClient.getMinutes();
int s = timeClient.getSeconds();

if(millis()-lastScroll>30000)
{
scrollMode=true;
lastScroll=millis();

String msg="  "+days[timeClient.getDay()]+"   ";

display.displayClear();
display.displayText(msg.c_str(),PA_LEFT,70,0,PA_SCROLL_LEFT,PA_SCROLL_LEFT);
}

if(scrollMode)
{
if(display.displayAnimate())
{
scrollMode=false;
display.displayClear();
}
return;
}

char buf[6];

if(s%2==0)
sprintf(buf,"%02d:%02d",h,m);
else
sprintf(buf,"%02d %02d",h,m);

display.setTextAlignment(PA_CENTER);
display.print(buf);

/* Auto brightness night mode */

if(h>=22 || h<=6)
display.setIntensity(1);
else
display.setIntensity(4);

delay(500);
}
