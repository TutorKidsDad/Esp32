#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include "pins.h"
#include "comms.h"
//#include "Adafruit_TCS34725.h"
#include <WiFi.h>
#include <HTTPClient.h>
 
#define THRESHOLD_WP -57.00
#define THRESHOLD_WP0 THRESHOLD_WP
#define THRESHOLD_WP1 THRESHOLD_WP
//#define THRESHOLD_WP2 THRESHOLD_WP
//#define THRESHOLD_WP3 THRESHOLD_WP

float v_batt, v_psu;
int v0, v1, v2, v3;
int pumpStatus = 0; // Global variable for pump status
int psu_swap_t, psu_en = 0;
int pump_en = 0; // turn this to 1 to activate the pump

void read_voltage();
void sendToThingSpeak(int valve0Status, int valve1Status, int pumpStatus);

void turn_relay_off()
{
  
  digitalWrite(PUMP, LOW);
 digitalWrite(VALVE0, LOW);
 digitalWrite(VALVE1, LOW);
 // digitalWrite(VALVE2, LOW);
 // digitalWrite(VALVE3, LOW);
}

void init_pins()
{
  pinMode(VALVE0, OUTPUT);
  pinMode(VALVE1, OUTPUT);
  //pinMode(VALVE2, OUTPUT);
  //pinMode(VALVE3, OUTPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(PWR_SW, OUTPUT);
  digitalWrite(PWR_SW, LOW);

  turn_relay_off();
  v_batt = 12.23;
}

void switch_valves()
{
    if(wp0 > THRESHOLD_WP0 && millis() - ts0 < TIMEOUT)
    {
      v0 = 1;
      digitalWrite(VALVE0, HIGH);
      Serial.println("Valve 0 High"); // Print the data to Serial Monitor
      
    }
    else
    {
      v0 = 0;
      digitalWrite(VALVE0, LOW);
      Serial.println("Valve 0 low"); // Print the data to Serial Monitor
    }
      
    if(wp1 > THRESHOLD_WP1 && millis() - ts1 < TIMEOUT)
    {
      v1 = 1;
      digitalWrite(VALVE1, HIGH);
      Serial.println("Valve 1 High"); // Print the data to Serial Monitor
      
    }
   else
    {
      v1 = 0;
      digitalWrite(VALVE1, LOW);
      Serial.println("Valve 1 low"); // Print the data to Serial Monitor
    }
/*
    if(wp2 > THRESHOLD_WP2 && millis() - ts2 < TIMEOUT)
    {
      v2 = 1;
      digitalWrite(VALVE2, HIGH);
    }
    else
    {
      v2 = 0;
      digitalWrite(VALVE2, LOW);
    }

    if(wp3 > THRESHOLD_WP3 && millis() - ts3 < TIMEOUT)
    {
      v3 = 1;
      digitalWrite(VALVE3, HIGH);
    }
    else
    {
      v3 = 0;
      digitalWrite(VALVE3, LOW);
    }
    */
}
void read_voltage()
{
  v_batt = (v_batt + analogRead(V_BATT) * 3.3 * 6.82 / 4096) / 2.0;
  v_psu = analogRead(V_PSU) * 3.3 * 7.60 / 4096;
  if (v_psu < 8.0)
    v_psu = 0.0;

  if (v_psu >= 10.5 && millis() - psu_swap_t > 5000 && psu_en == 0)
  {
    psu_en = 1;
    psu_swap_t = millis();
    turn_relay_off();
    delay(300);
    digitalWrite(PWR_SW, HIGH);
    delay(300);
    switch_valves();
    //Serial.println("PSU Swapped");
  }
  else if (v_psu < 10.5)
  {
    psu_en = 0;
    digitalWrite(PWR_SW, LOW);
    Serial.println("Battery low");
  }
  Serial.println("v_batt : " + String(v_batt) + " v_psu : " + String(v_psu));
}


/*
void read_voltage()
{
  v_batt = (v_batt + analogRead(V_BATT)*3.3*6.82 /4096)/2.0;
  v_psu  = analogRead(V_PSU)*3.3*7.60/4096;
  if(v_psu < 8.0)
    v_psu = 0.0;

  Serial.println(String(v_batt)+" "+String(analogRead(V_BATT))+" "+String(analogRead(V_PSU))+" "+String(v_psu));
  
  if(v_psu >= 10.5 && millis() - psu_swap_t > 5000 && psu_en == 0)
  {
    psu_en = 1;
    psu_swap_t = millis();
    turn_relay_off();
    delay(300);
    digitalWrite(PWR_SW, HIGH);
    delay(300);     
    switch_valves();
    //Serial.println("PSU Swapped");
  }
  else if(v_psu < 10.5)
  {
    psu_en = 0;
    digitalWrite(PWR_SW, LOW);
    //Serial.println("Battery Swapped");
  }
  Serial.println("v_batt : "+String(v_batt)+" v_psu : "+String(v_psu));
}
*/

void check_pump()
{
  if (v0 == 1 || v1 == 1)
  {
    digitalWrite(PUMP, HIGH);
    pumpStatus = 1; // Set global pumpStatus
    Serial.println("Pump on");
    delay(180000); //pump on for min 3 min
  }
  else
  {
    digitalWrite(PUMP, LOW);
    pumpStatus = 0; // Set global pumpStatus
    Serial.println("Pump off");
  }

  sendToThingSpeak(v0, v1, pumpStatus);
}

/*
void check_pump()
{
  int pumpStatus = 0;
  if (v0 == 1 || v1 == 1)
  {
    digitalWrite(PUMP, HIGH);
    pumpStatus = 1;
    Serial.println("Pump on");
  }
  else
  {
    digitalWrite(PUMP, LOW);
    Serial.println("Pump off");
  }

  sendToThingSpeak(v0, v1, pumpStatus);
}
*/


void sendToThingSpeak(int valve0Status, int valve1Status, int pumpStatus)
{
  String url = String(THINGSPEAK_WRITE_URL) +
               "&field1=" + String(valve0Status) +
               "&field2=" + String(valve1Status) +
               "&field3=" + String(pumpStatus);

    Serial.println("url sent to ThingSpeak");
    Serial.println(url);            

  HTTPClient http;
  http.begin(url);

  int httpCode = http.GET();

  if (httpCode > 0)
  {
    String payload = http.getString();
    Serial.println("Data sent to ThingSpeak");
    Serial.println(payload);
  }
  else
  {
    Serial.println("Error in sending data");
  }

  http.end();
}

#endif
