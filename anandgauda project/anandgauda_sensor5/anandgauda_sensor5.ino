/*https://github.com/espressif/esptool to flash 
 * PINCONFIG ACCORDING TO ESP32S WROOM
 * 
 * //esptool.py erase_flash
ESP8266/ESP32 publish the RSSI as the WiFi signal strength to ThingSpeak channel.
This example is for explaining how to use the AutoConnect library.
readHumidity
In order to execute this example, the ThingSpeak account is needed. Sign up
for New Use
r Account and create a New Channel via My Channels.
For details, please refer to the project page.
https://hieromon.github.io/AutoConnect/howtoembed.html#used-with-mqtt-as-a-client-application

v
/libraries/AutoConnect/src/AutoConnectDefs.h               ** TO CHANGE CREDENTIALS LIKE AP NAME AND PASSWORD
/libraries/AutoConnect/src/AutoConnectLabels.h               ** TO CHANGE LABLES OF FRONT PAGE

This example is based on the environment as of March 20, 2018.
Copyright (c) 2020 Hieromon Ikasamo.
This software is released under the MIT License.
https://opensource.org/licenses/MIT
*/
/////////////////////////////////////////////////////DO NOT DISTURB START ///////////////////////////
#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define GET_CHIPID()  (ESP.getChipId())
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#define GET_CHIPID()  ((uint16_t)(ESP.getEfuseMac()>>32))
#endif
#include <PubSubClient.h>
#include <ThingSpeak.h>
//#include <DHT.h>
#include "math.h"
//#include <LiquidCrystal_I2C.h>
//#include <math.h>
#include <WiFiClient.h>
#include <WiFiMulti.h>

WiFiMulti wifiMulti;
#include "ThingSpeak.h"
#include "math.h"

//const char* ssid = "testing";   // your network SSID (name) 
//const char* password = "password";   // your network password

WiFiClient  client;
/*
unsigned long myChannelNumber = 1441546; // CHANNEL 1
const char * myWriteAPIKey = "XD7FZZ8KA7XBELKF";
*/
unsigned long myChannelNumber = 1441547; // CHANNEL 2
const char * myWriteAPIKey = "44DYW7JF3MUKPXBE";


// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 45000;
////////////////////////
/////////////////////////////////////////////////////DO NOT DISTURB STOP ///////////////////////////


#define NUM_READS 10    // Number of sensor reads for filtering

typedef struct {        // Structure to be used in percentage and resistance values matrix to be filtered (have to be in pairs)
  int moisture1;
  long resistance1;
   int moisture2;
  long resistance2;
} values;
/*
/////////////////SERIAL LCD START /////////////////////////
// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

//////////////////// SERIAL LCD STOP////////////////////////

// define Ohm character
byte ohm[8] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B01010,
  B11011,
  B00000
};
//////////////////////
*/
const long knownResistor1 = 4700;  // Constant value of known resistor in Ohms

int activeDigitalPin1 = 18;         // 6 or 7 interchangeably
int supplyVoltageAnalogPin1;       // 6-ON: A0, 7-ON: A1
int sensorVoltageAnalogPin1;       // 6-ON: A1, 7-ON: A0

int supplyVoltage1;                // Measured supply voltage
int sensorVoltage1;                // Measured sensor voltage

values valueOf[NUM_READS];        // Calculated moisture percentages and resistances to be sorted and filtered

int i;                            // Simple index variable

////////////
//////////////////////
const long knownResistor2 = 4700;  // Constant value of known resistor in Ohms

int activeDigitalPin2 = 16;         // 6 or 7 interchangeably
int supplyVoltageAnalogPin2;       // 6-ON: A0, 7-ON: A1
int sensorVoltageAnalogPin2;       // 6-ON: A1, 7-ON: A0

int supplyVoltage2;                // Measured supply voltage
int sensorVoltage2;                // Measured sensor voltage

//values valueOf[NUM_READS];        // Calculated moisture percentages and resistances to be sorted and filtered

//int i;                            // Simple index variable
/*
////////////
#define DHTTYPE DHT11
#define DHTPIN  27   // DHT22 data pin connects to D4
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266


float prevTemp = 0;
int sent = 0;
*/
///////////////////////////////////FOR SLEEP FUNCTION///////////////////
#define mS_TO_S_FACTOR 1000  /* Conversion factor for mili seconds to seconds */
#define TIME_TO_SLEEP  300000    /* Time ESP32 will go to sleep (in seconds) */
RTC_DATA_ATTR int bootCount = 0;
int wifiCount = 0;
///////////////////////////////////SLEEP FN STOP /////////////////
int GREEN_LED_PIN = 2;
/*
#include <Wire.h>
    #include <Arduino.h>
    #include <OneWire.h>
    #include <DallasTemperature.h>

 //      #define myPeriodic 15 // Seconds
    #define ONE_WIRE_BUS 26 // DS18B20 on arduino pin2 corresponds to D2 on physical board

    OneWire oneWire(ONE_WIRE_BUS);
    // DallasTemperature DS18B20(&oneWire);
   DallasTemperature sensors(&oneWire);
///////////////////////
*/
#define Wifi_Status 15 //  D15   Wifi Status

/*
  AC_USE_SPIFFS indicates SPIFFS or LittleFS as available file systems that
  will become the AUTOCONNECT_USE_SPIFFS identifier and is exported as showing
  the valid file system. After including AutoConnect.h, the Sketch can determine
  whether to use FS.h or LittleFS.h by AUTOCONNECT_USE_SPIFFS definition.
*/


void setupCurrentPath() {
  if ( activeDigitalPin1 == 18 ) {
    activeDigitalPin1 = 19;
    supplyVoltageAnalogPin1 = 34;
    sensorVoltageAnalogPin1 = 35;
     //   Serial.println("sensor1-1");

  }
  else {
    activeDigitalPin1 = 18;
    supplyVoltageAnalogPin1 = 35;
    sensorVoltageAnalogPin1 = 34;
  //  Serial.println("sensor1-2");
  }

   if ( activeDigitalPin2 == 16 ) {
    activeDigitalPin2 = 17;
    supplyVoltageAnalogPin2 = 32;
    sensorVoltageAnalogPin2 = 33;
 //   Serial.println("sensor2-1");
  }
  else {
    activeDigitalPin2 = 16;
    supplyVoltageAnalogPin2 = 33;
    sensorVoltageAnalogPin2 = 32;
  //  Serial.println("sensor2-2");
  }
}

// Selection sort algorithm
void sortMoistures() {
  int j;
  values temp;
  for(i=0; i<NUM_READS-1; i++)
    for(j=i+1; j<NUM_READS; j++)
      if ( valueOf[i].moisture1 > valueOf[j].moisture1 ) {
        temp = valueOf[i];
        valueOf[i] = valueOf[j];
        valueOf[j] = temp;
      }
  delay(100);

  for(i=0; i<NUM_READS-1; i++)
    for(j=i+1; j<NUM_READS; j++)
      if ( valueOf[i].moisture2 > valueOf[j].moisture2 ) {
        temp = valueOf[i];
        valueOf[i] = valueOf[j];
        valueOf[j] = temp;
      }  
        delay(100);
 
}

////////////////////////////////

void setup() {

  //////////////////////////////////
  WiFi.mode(WIFI_STA);   
      wifiMulti.addAP("testing", "password");
    wifiMulti.addAP("swapnil2@4", "TPS@462016");
    wifiMulti.addAP("OnlineTPS", "TPS@462014");

    Serial.println("Connecting Wifi...");
    if(wifiMulti.run() == WL_CONNECTED) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
    }
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  
   pinMode(Wifi_Status, OUTPUT);    // LED pin as output.
  pinMode(GREEN_LED_PIN,OUTPUT);
  digitalWrite(Wifi_Status, LOW);
  digitalWrite(GREEN_LED_PIN,HIGH);
  delay(500);
 // digitalWrite(GREEN_LED_PIN,HIGH);
 // delay(1000);
 ////////////////////////////////////////////////// LCD START//////////////////////////////
 /*
   // initialize LCD
    lcd.begin(16,2);
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
// set up the LCD's number of columns and rows: 
  lcd.createChar(1, ohm);
  lcd.begin(16, 2);
  lcd.print ("Soil Moisture Sensor");
 lcd.setCursor(0, 1);
  // print the results to the serial monitor:
  lcd.print("Initializing Sensor..." ); 
     delay(1000);

  ////////////////////////////////////////////////LCD STOP ///////////////////////////////////
  */
  // initialize the digital pin as an output.
  // Pin 6 is sense resistor voltage supply 1
  pinMode(18, OUTPUT);    

  // initialize the digital pin as an output.
  // Pin 7 is sense resistor voltage supply 2
  pinMode(19, OUTPUT);   


    // initialize the digital pin as an output.
  // Pin 6 is sense resistor voltage supply 1
  pinMode(16, OUTPUT);    

  // initialize the digital pin as an output.
  // Pin 7 is sense resistor voltage supply 2
  pinMode(17, OUTPUT);  

  delay(500);  
/////////////////////////////////////////////////////DO NOT DISTURB START ///////////////////////////
  Serial.begin(115200);
 // dht.begin();
  Serial.println();
// sensors.begin();
   pinMode(Wifi_Status, OUTPUT);    // LED pin as output.
  delay(100);
    digitalWrite(Wifi_Status, LOW);
      delay(100);
    Serial.println("TRYING WIFI");
      delay(1000);
 dataprocess();
   delay(500);
   Serial.flush();
   Serial.println("Sleeping");
   delay(100);
  /////////////////////////////////////////////
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * mS_TO_S_FACTOR);
  esp_deep_sleep_start();

/////////////////////////////////////////////////////DO NOT DISTURB STOP ///////////////////////////
    
}
void loop() 
{
 
}
void dataprocess() 
{
/////////////////////////////////////////////////////DO NOT DISTURB START ///////////////////////////
   int statusCode = 0;
   delay(5000);
   if(wifiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
        delay(1000);
    }
  // Check the status of the read operation to see if it was successful
  statusCode = ThingSpeak.getLastReadStatus();
  if (statusCode == 200) {
    digitalWrite(Wifi_Status, HIGH);
    digitalWrite(GREEN_LED_PIN,LOW);
        Serial.println("CONNECTED TO WIFI");
        delay(500);

    }
     else {
    Serial.println("Problem reading channel. HTTP error code " + String(statusCode));
   digitalWrite(Wifi_Status, LOW);
   digitalWrite(GREEN_LED_PIN,HIGH);
           delay(500);

     }
     
     
/////////////////////////////////////////////////////DO NOT DISTURB STOP ///////////////////////////
/*
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float Tempsensor1 = sensors.getTempCByIndex(0);
 float Tempsensor2 = sensors.getTempCByIndex(1);
  // Check if reading was successful
if((Tempsensor1 != DEVICE_DISCONNECTED_C) || (Tempsensor2 != DEVICE_DISCONNECTED_C))
 //if(tempC2 != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.println(Tempsensor1);
    Serial.print("Temperature for the device 2 (index 1) is: ");
    Serial.println(Tempsensor2);
  } 
  else
  {
    Serial.println("Error: Could not read temperature data");
  }
*/
  ////////////////////////////////////////////////////////////////////////////////////////////////
 // read sensor, filter, and calculate resistance value
  // Noise filter: median filter

  for (i=0; i<NUM_READS; i++) {

    setupCurrentPath();      // Prepare the digital and analog pin values

    // Read 1 pair of voltage values
    digitalWrite(activeDigitalPin1, HIGH);                 // set the voltage supply on
   delay(10);
   supplyVoltage1 = analogRead(supplyVoltageAnalogPin1);   // read the supply voltage
    sensorVoltage1 = analogRead(sensorVoltageAnalogPin1);   // read the sensor voltage
    digitalWrite(activeDigitalPin1, LOW);                  // set the voltage supply off  
   delay(10);
    // Calculate resistance and moisture percentage without overshooting 100
    // the 0.5 add-term is used to round to the nearest integer
    // Tip: no need to transform 0-1023 voltage value to 0-5 range, due to following fraction
    valueOf[i].resistance1 = long( float(knownResistor1) * ( supplyVoltage1 - sensorVoltage1 ) / sensorVoltage1 + 0.5 );
   delay(10);
    valueOf[i].moisture1 = min( int( pow( valueOf[i].resistance1/31.65 , 1.0/-1.695 ) * 400 + 0.5 ) , 100 );
   delay(10);
//  valueOf[i].moisture = min( int( pow( valueOf[i].resistance/331.55 , 1.0/-1.695 ) * 100 + 0.5 ) , 100 );

delay(200);
 // Read 1 pair of voltage values
    digitalWrite(activeDigitalPin2, HIGH);                 // set the voltage supply on
   delay(10);
   supplyVoltage2 = analogRead(supplyVoltageAnalogPin2);   // read the supply voltage
    sensorVoltage2 = analogRead(sensorVoltageAnalogPin2);   // read the sensor voltage
    digitalWrite(activeDigitalPin2, LOW);                  // set the voltage supply off  
   delay(10);
    // Calculate resistance and moisture percentage without overshooting 100
    // the 0.5 add-term is used to round to the nearest integer
    // Tip: no need to transform 0-1023 voltage value to 0-5 range, due to following fraction
    valueOf[i].resistance2 = long( float(knownResistor2) * ( supplyVoltage2 - sensorVoltage2 ) / sensorVoltage2 + 0.5 );
   delay(10);
    valueOf[i].moisture2 = min( int( pow( valueOf[i].resistance2/31.65 , 1.0/-1.695 ) * 400 + 0.5 ) , 100 );
   delay(10);
//  valueOf[i].moisture = min( int( pow( valueOf[i].resistance/331.55 , 1.0/-1.695 ) * 100 + 0.5 ) , 100 );

delay(200);
  }

  // end of multiple read loop

  // Sort the moisture-resistance vector according to moisture
  sortMoistures();
delay(1000);

  // Print out median values
  Serial.print("sensor1 resistance = ");
  Serial.println(valueOf[NUM_READS/2].resistance1);
    delay(25); 

    Serial.print("sensor1 moisture % = ");
  Serial.println(valueOf[NUM_READS/2].moisture1);

    delay(25); 

    // Print out median values
  Serial.print("sensor2 resistance = ");
  Serial.println(valueOf[NUM_READS/2].resistance2);
    delay(25); 

    Serial.print("sensor2 moisture % = ");
  Serial.println(valueOf[NUM_READS/2].moisture2);
      delay(25); 
   Serial.print("supplyVoltage 1= ");
  Serial.println(supplyVoltage1);
     delay(25); 
    Serial.print("sensorVoltage 1 = ");
  Serial.println(sensorVoltage1);
     delay(25); 
     Serial.print("supplyVoltage 2= ");
  Serial.println(supplyVoltage2);
     delay(25); 
    Serial.print("sensorVoltage 2= ");
  Serial.println(sensorVoltage2);
   delay(25); 

/*
float AirTemperature,humidity;
  //char buffer[10];
  humidity = dht.readHumidity();          // Read humidity (percent)
  delay(100);
  //t1 = dht.readTemperature(true);     // Read temperature as Fahrenheit
  AirTemperature = dht.readTemperature();     // Read temperature as cel
  delay(100);
    // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(AirTemperature)) {
     Serial.println("Failed to read from DHT sensor!");
    //  return;
  }
 // Serial.print(String(sent)+" Temperature: ");
  Serial.println(AirTemperature);
  delay(100);
 // Serial.print(String(sent)+" Humidity: ");
  Serial.println(humidity);
    delay(100);
 //   Serial.println("I am Connected 1") ;
*/
   unsigned long currentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && ((millis() - lastTime) > timerDelay) ) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    lastTime = currentMillis;
  }
/////////////////////////////////////////////////////DO NOT DISTURB START ///////////////////////////
     // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeField(myChannelNumber, 9, valueOf[NUM_READS/2].moisture1, myWriteAPIKey);
    if(x == 200){
      Serial.println("Channel 9 update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
delay(40000);

    int y = ThingSpeak.writeField(myChannelNumber,10, valueOf[NUM_READS/2].moisture2, myWriteAPIKey);
    if(y == 200){
      Serial.println("Channel 10 update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(y));
    }
delay(40000);
/////////////////////////////////////////////////////DO NOT DISTURB STOP ///////////////////////////

}
