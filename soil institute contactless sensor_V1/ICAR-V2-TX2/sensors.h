#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include "pins.h"
#include <Adafruit_AHTX0.h>
#include "Adafruit_TCS34725.h"

#define THRESHOLD1 50
#define THRESHOLD2 75

Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

bool strip_present = false;
String strip = "FF0A08";
//int moisture=0;
//float wp=0, humidity=0, temp=0, vbatt=3.9;

void init_pins()
{
  pinMode(BUZZER, OUTPUT);
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);
  pinMode(STRIP_LED, OUTPUT);
  pinMode(V_BATT ,INPUT);
  
}
/*
void init_sensors()
{
  if (!aht.begin()) 
    Serial.println("Failed to find AHT10/AHT20 chip");
  delay(1000);
  if (!tcs.begin())
    Serial.println("No TCS34725 found ... check your connections");
   delay(1000);
  aht_temp = aht.getTemperatureSensor();
  aht_temp->printSensorDetails();
delay(1000);
  aht_humidity = aht.getHumiditySensor();
  aht_humidity->printSensorDetails();
  delay(1000);
}*/
void init_sensors() {
    if (!aht.begin()) {
        Serial.println("Failed to find AHT10/AHT20 chip");
    delay(1000);
    } else {
        aht_temp = aht.getTemperatureSensor();
        aht_temp->printSensorDetails();
        aht_humidity = aht.getHumiditySensor();
        aht_humidity->printSensorDetails();
    }

    if (!tcs.begin()) {
        Serial.println("No TCS34725 found ... check your connections");
    delay(1000);
    }
}
void read_strip()
{
  uint16_t r, g, b, c, colorTemp, lux;
  digitalWrite(STRIP_LED, HIGH);
  delay(1000);
  String hexcode;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
  Serial.println("R = "+ String(r) + " G = "+ String(g) + " B = "+String(b) + " L = "+String(lux));
  r = map(r, 0, 4095, 0, 255);
  g = map(g, 0, 4095, 0, 255);
  b = map(b, 0, 4095, 0, 255);
  Serial.println("R = "+ String(r) + " G = "+ String(g) + " B = "+String(b) + " L = "+String(lux));
  strip = "";
  hexcode = String(r, HEX).length() == 2 ? String(r, HEX) : "0"+String(r, HEX);
  strip += hexcode;
  hexcode = String(g, HEX).length() == 2 ? String(g, HEX) : "0"+String(g, HEX);
  strip += hexcode;
  hexcode = String(b, HEX).length() == 2 ? String(b, HEX) : "0"+String(b, HEX);
  strip += hexcode;
  digitalWrite(STRIP_LED, LOW);
  delay(1000);

}
void update_led()
{
  if(humidity > 0 && humidity <= THRESHOLD1)
  {
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, LOW);
    digitalWrite(B_LED, LOW);
  }
  else if(humidity > THRESHOLD1 && humidity <= THRESHOLD2)
  {
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, HIGH);
    digitalWrite(B_LED, LOW);
  }
  else
  {
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, HIGH);
    digitalWrite(B_LED, LOW);
  }
}


// Define other necessary global variables and constants

void read_data() {
    if (!aht_temp || !aht_humidity) {
        Serial.println("AHT sensor not initialized");
        delay(1000);
       return;
    }

    sensors_event_t h, t;
    aht_humidity->getEvent(&h);
    delay(1000);
    aht_temp->getEvent(&t);
    delay(1000);

    temp = t.temperature;
    delay(1000);
    humidity = h.relative_humidity;
    delay(1000);

    int temp_k = temp + 273;
    wp = ((8.314 * temp_k) / (0.000018)) * log(1 / humidity) * pow(10, -7);
    if (DEV_ID == 2) wp = wp + 2;

    update_led();

    if (tcs.begin()) {
        read_strip();
    } else {
        Serial.println("TCS34725 sensor not initialized");
    }

    vbatt = analogRead(V_BATT) / 4096.0 * 3.3 * 1.89;
    delay(100);
    moisture = analogRead(MOISTURE_IN);
    delay(100);
    moisture = map(moisture, 0, 4095, 0, 100);
    delay(100);

    Serial.println("Moisture : " + String(moisture));
    Serial.println("Temp     : " + String(temp));
    Serial.println("RH       : " + String(humidity));
    Serial.println("Strip    : " + strip);
    Serial.println("vbatt    : " + String(vbatt));
}

/*
void read_data()
{
  sensors_event_t h, t;
  
  int temp_k = temp + 273;
  wp = ((8.314*temp_k)/(0.000018))*log(1/humidity) * pow(10,-7);
  if(DEV_ID == 2)
    wp = wp + 2;
  update_led();
delay(1000);
  aht_humidity->getEvent(&h);
  aht_temp->getEvent(&t);
delay(1000);
  temp = (t.temperature);
  humidity = (h.relative_humidity);
  read_strip();
  vbatt = analogRead(V_BATT)/ 4096.0 * 3.3 * 1.89;  
delay(1000);
  moisture = analogRead(MOISTURE_IN);
  moisture = map(moisture, 0, 4095, 0, 100);
  delay(1000);
  Serial.println("Moisture : "+String(moisture));
  Serial.println("Temp     : "+String(temp));
  Serial.println("RH       : "+String(humidity));
  Serial.println("Strip    : "+strip);
  Serial.println("vbatt       : "+String(vbatt));
}
*/
#endif
