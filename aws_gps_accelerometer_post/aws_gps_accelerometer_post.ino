//tools/cpufreq-160mhz
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>
#ifdef ESP32
#include <WiFi.h>
#elif defined(ESP8266)TinyGPS++
#include <ESP8266WiFi.h>
#else
#error Platform not supported
#endif
#include <WiFiClientSecure.h>
#include <MQTT.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson (use v6.xx)
#include <time.h>
#define emptyString String()

//Follow instructions from https://github.com/debsahu/ESP-MQTT-AWS-IoT-Core/blob/master/doc/README.md
//Enter values in secrets.h ▼
#include "secrets.h"


#if !(ARDUINOJSON_VERSION_MAJOR == 6 and ARDUINOJSON_VERSION_MINOR >= 7)
#error "Install ArduinoJson v6.7.0-beta or higher"
#endif

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

const int MQTT_PORT = 8883;
const char MQTT_SUB_TOPIC[] = "$aws/things/" THINGNAME "/shadow/update";
const char MQTT_PUB_TOPIC[] = "$aws/things/" THINGNAME "/shadow/update";

float xAccl, yAccl, zAccl,aspeed;
double latitude=0;
double longitude=0;

// The TinyGPS++ object
TinyGPSPlus gps;
    

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);



#ifdef USE_SUMMER_TIME_DST
uint8_t DST = 1;
#else
uint8_t DST = 0;
#endif

WiFiClientSecure net;

#ifdef ESP8266
BearSSL::X509List cert(cacert);
BearSSL::X509List client_crt(client_cert);
BearSSL::PrivateKey key(privkey);
#endif

MQTTClient client;

unsigned long lastMillis = 0;
time_t now;
time_t nowish = 1510592825;

void NTPConnect(void)
{
  Serial.print("Setting time using SNTP");
  configTime(TIME_ZONE * 3600, DST * 3600, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < nowish)
  {
    delay(1000);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("done!");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void messageReceived(String &topic, String &payload)
{
  Serial.println("Recieved [" + topic + "]: " + payload);
}

void lwMQTTErr(lwmqtt_err_t reason)
{
  if (reason == lwmqtt_err_t::LWMQTT_SUCCESS)
    Serial.print("Success");
  else if (reason == lwmqtt_err_t::LWMQTT_BUFFER_TOO_SHORT)
    Serial.print("Buffer too short");
  else if (reason == lwmqtt_err_t::LWMQTT_VARNUM_OVERFLOW)
    Serial.print("Varnum overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_CONNECT)
    Serial.print("Network failed connect");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_TIMEOUT)
    Serial.print("Network timeout");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_READ)
    Serial.print("Network failed read");
  else if (reason == lwmqtt_err_t::LWMQTT_NETWORK_FAILED_WRITE)
    Serial.print("Network failed write");
  else if (reason == lwmqtt_err_t::LWMQTT_REMAINING_LENGTH_OVERFLOW)
    Serial.print("Remaining length overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_REMAINING_LENGTH_MISMATCH)
    Serial.print("Remaining length mismatch");
  else if (reason == lwmqtt_err_t::LWMQTT_MISSING_OR_WRONG_PACKET)
    Serial.print("Missing or wrong packet");
  else if (reason == lwmqtt_err_t::LWMQTT_CONNECTION_DENIED)
    Serial.print("Connection denied");
  else if (reason == lwmqtt_err_t::LWMQTT_FAILED_SUBSCRIPTION)
    Serial.print("Failed subscription");
  else if (reason == lwmqtt_err_t::LWMQTT_SUBACK_ARRAY_OVERFLOW)
    Serial.print("Suback array overflow");
  else if (reason == lwmqtt_err_t::LWMQTT_PONG_TIMEOUT)
    Serial.print("Pong timeout");
}

void lwMQTTErrConnection(lwmqtt_return_code_t reason)
{
  if (reason == lwmqtt_return_code_t::LWMQTT_CONNECTION_ACCEPTED)
    Serial.print("Connection Accepted");
  else if (reason == lwmqtt_return_code_t::LWMQTT_UNACCEPTABLE_PROTOCOL)
    Serial.print("Unacceptable Protocol");
  else if (reason == lwmqtt_return_code_t::LWMQTT_IDENTIFIER_REJECTED)
    Serial.print("Identifier Rejected");
  else if (reason == lwmqtt_return_code_t::LWMQTT_SERVER_UNAVAILABLE)
    Serial.print("Server Unavailable");
  else if (reason == lwmqtt_return_code_t::LWMQTT_BAD_USERNAME_OR_PASSWORD)
    Serial.print("Bad UserName/Password");
  else if (reason == lwmqtt_return_code_t::LWMQTT_NOT_AUTHORIZED)
    Serial.print("Not Authorized");
  else if (reason == lwmqtt_return_code_t::LWMQTT_UNKNOWN_RETURN_CODE)
    Serial.print("Unknown Return Code");
}

void connectToMqtt(bool nonBlocking = false)
{
  Serial.print("MQTT connecting ");
  
  while (!client.connected())
  {
    if (client.connect(THINGNAME))
    {
      Serial.println("connected!");
      if (!client.subscribe(MQTT_SUB_TOPIC))
        lwMQTTErr(client.lastError());
    }
    else
    {
      #ifdef ESP8266
      Serial.print("SSL Error Code: ");
      Serial.println(net.getLastSSLError());
      #endif

      Serial.print("failed, reason -> ");
      lwMQTTErrConnection(client.returnCode());
      if (!nonBlocking)
      {
        Serial.println(" < try again in 5 seconds");
        delay(5000);
      }
      else
      {
        Serial.println(" <");
      }
    }
    if (nonBlocking)
      break;
  }
}


void connectToWiFi(String init_str)
{
  if (init_str != emptyString)
    Serial.print(init_str);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  if (init_str != emptyString)
    Serial.println("ok!");
}

void checkWiFiThenMQTT(void)
{
  connectToWiFi("Checking WiFi");
  connectToMqtt();
}

unsigned long previousMillis = 0;
const long interval = 5000;

void checkWiFiThenMQTTNonBlocking(void)
{
  connectToWiFi(emptyString);
  if (millis() - previousMillis >= interval && !client.connected()) {
    previousMillis = millis();
    connectToMqtt(true);
  }
}

void checkWiFiThenReboot(void)
{
  connectToWiFi("Checking WiFi");
  Serial.print("Rebooting");
  ESP.restart();
}

void sendData(void)
{
  DynamicJsonDocument jsonBuffer(JSON_OBJECT_SIZE(3) + 100);
  JsonObject root = jsonBuffer.to<JsonObject>();
  JsonObject state = root.createNestedObject("state");
  JsonObject state_reported = state.createNestedObject("reported");
  //state_reported["value"] = random(100);
 // state_reported["value"] = String(latitude, 6)+String(comma)+String(longitude, 6)+String(comma)+aspeed+String(comma)+xAccl+String(comma)+yAccl+String(comma)+zAccl;
  state_reported["value"] =String((gps.location.lat()), 6)+String(comma)+String((gps.location.lng()), 6)+String(comma)+aspeed+String(comma)+xAccl+String(comma)+yAccl+String(comma)+zAccl;
  
  Serial.printf("Sending  [%s]: ", MQTT_PUB_TOPIC);
  serializeJson(root, Serial);
  Serial.println();
  char shadow[measureJson(root) + 1];
  serializeJson(root, shadow, sizeof(shadow));
  if (!client.publish(MQTT_PUB_TOPIC, shadow, false, 0))
    lwMQTTErr(client.lastError());
}

void setup()
{
  Serial.begin(9600);
  delay(5000);
  Serial.println();
  Serial.println();
#ifdef ESP32
  WiFi.setHostname(THINGNAME);
#else
  WiFi.hostname(THINGNAME);
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  connectToWiFi(String("Attempting to connect to SSID: ") + String(ssid));

  NTPConnect();

#ifdef ESP32
  net.setCACert(cacert);
  net.setCertificate(client_cert);
  net.setPrivateKey(privkey);
#else
  net.setTrustAnchors(&cert);
  net.setClientRSACert(&client_crt, &key);
#endif

  client.begin(MQTT_HOST, MQTT_PORT, net);
  client.onMessage(messageReceived);

  connectToMqtt();

 Serial.begin(9600);
  ss.begin(GPSBaud);
  Serial.println(F(""));
  Serial.println(F("A simple demonstration of GPS with an attached GPS module"));
  Serial.print(F("Testing ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println();


 Serial.begin(9600);
 Serial.println("Accelerometer Test"); Serial.println("");
 
 // Initialise the sensor 
 if(!accel.begin())
 {
 // There was a problem detecting the ADXL345 ... check your connections 
 Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
 while(1);
 }

 accel.setRange(ADXL345_RANGE_2_G);
 
 

 Serial.println("");

  
}


void displayInfo()
{
  
 Serial.print(F("Location: ")); 
 //if (gps.location.isValid())
  //{
//float temp = ((analogRead(A0) * resolution) * 100)+23.89;

    double latitude = (gps.location.lat());
    double longitude = (gps.location.lng());
    aspeed=gps.speed.kmph();
    
    String latbuf;
    latbuf += (String(latitude, 6));
    Serial.print("latitude: ");Serial.println(latbuf);

    String lonbuf;
    lonbuf += (String(longitude, 6));
    Serial.print("longitude: ");Serial.println(lonbuf);

Serial.print("Speed: ");Serial.println(gps.speed.kmph()); // Speed in kilometers per hour (double)

  //{
    Serial.print(F("GPS DATE TIME"));
 // }

  Serial.print(F("  Date/Time: "));
  //if (gps.date.isValid())
 // {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  //}
  //else
  //{
    Serial.print(F("Date"));
 // }

  Serial.print(F(" "));
  
    Serial.print("Time UTC  : ");
  Serial.print(gps.time.hour());                       // GPS time UTC 
  Serial.print(":");
  Serial.print(gps.time.minute());                     // Minutes
  Serial.print(":");
  Serial.println(gps.time.second()); 
  //}
  //else
  //{
    Serial.print(F("Time"));
 // }

  Serial.println();
}






///////////////////////////////////////
void loop()
{
  now = time(nullptr);
  if (!client.connected())
  {
    checkWiFiThenMQTT();
    //checkWiFiThenMQTTNonBlocking();
    //checkWiFiThenReboot();
  }
  else
  {
////////////////////////////

 /* Get a new sensor event */ 
sensors_event_t event; 
accel.getEvent(&event);
xAccl=event.acceleration.x;
yAccl=event.acceleration.y;
zAccl=event.acceleration.z;


 
 /* Display the results (acceleration is measured in m/s^2) */
Serial.print("X: "); Serial.print(xAccl); Serial.print(" ");
 Serial.print("Y: "); Serial.print(yAccl); Serial.print(" ");
Serial.print("Z: "); Serial.print(zAccl); Serial.print(" ");Serial.println("m/s^2 ");
 
 

     displayInfo();
       delay(100); 
      smartDelay(600);                                      // Run Procedure smartDelay  
  if (millis() > 500 && gps.charsProcessed() < 10)
   Serial.println(F("No GPS data received: check wiring"));
 /*if (millis() > 1000 && gps.charsProcessed() < 20)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }*/
////////////////////////////

    
    client.loop();
    if (millis() - lastMillis > 1000)
    {
      lastMillis = millis();
      sendData();
    }
  }
}


///////////////////////////////////////////////////////////
static void smartDelay(unsigned long ms)                // This custom version of delay() ensures that the gps object is being "fed".
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } 
  while (millis() - start < ms);
}
