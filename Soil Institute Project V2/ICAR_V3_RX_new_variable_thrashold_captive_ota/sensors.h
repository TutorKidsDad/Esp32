#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include "pins.h"
#include "comms.h"
//#include "Adafruit_TCS34725.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
//#define THRESHOLD_WP -57.00
#define THRESHOLD_WP0 THRESHOLD_WP
#define THRESHOLD_WP1 THRESHOLD_WP
//#define THRESHOLD_WP2 THRESHOLD_WP
//#define THRESHOLD_WP3 THRESHOLD_WP
/*
// LCD dimensions
#define LCD_WIDTH 20
#define LCD_HEIGHT 4

// Create an LCD object with I2C address 0x27 (adjust if necessary)
LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);
*/
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
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  turn_relay_off();
  v_batt = 12.23;
}

void switch_valves()
{
    if(wp0 > field1Value && millis() - ts0 < TIMEOUT)
    {
      v0 = 1;
      digitalWrite(VALVE0, HIGH);
      digitalWrite(G_LED, HIGH);
      Serial.println("Valve 0 High"); // Print the data to Serial Monitor
      delay(1000);
    }
    else
    {
      v0 = 0;
      digitalWrite(VALVE0, LOW);
      Serial.println("Valve 0 low"); // Print the data to Serial Monitor
      delay(1000);
    }
      
    if(wp1 > field2Value && millis() - ts1 < TIMEOUT)
    {
      v1 = 1;
      digitalWrite(VALVE1, HIGH);
      digitalWrite(B_LED, HIGH);
      Serial.println("Valve 1 High"); // Print the data to Serial Monitor
      delay(1000);
    }
   else
    {
      v1 = 0;
      digitalWrite(VALVE1, LOW);
      Serial.println("Valve 1 low"); // Print the data to Serial Monitor
      delay(1000);
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

void check_pump()
{
  if (v0 == 1 || v1 == 1)
  {
    delay(2000);
    digitalWrite(PUMP, HIGH);
    digitalWrite(R_LED, HIGH);
    pumpStatus = 1; // Set global pumpStatus
    Serial.println("Pump on for 3 min");
    lcd.clear();
   lcd.backlight(); // Turn on the backlight
   /////////////
      // Print v0 and v1 status
    lcd.setCursor(0, 0);
    lcd.print("Status of Valves: ");
        lcd.setCursor(0, 1);
    lcd.print("v0: " + String(v0) + " v1: " + String(v1));
   /////////////
      lcd.setCursor(0, 2);
    lcd.print("Pump: " + String(pumpStatus ? "ON" : "OFF") + "  for 3 min");
       lcd.setCursor(0, 3);
    lcd.print("Will sleep then...");
                delay(15000);
        lcd.noBacklight(); // Turn off the backlight
    delay(180000); //pump on for min 3 min
  }
  else
  {
    digitalWrite(PUMP, LOW);
    pumpStatus = 0; // Set global pumpStatus
    Serial.println("Pump off");
          lcd.setCursor(0, 3);
          lcd.backlight(); // Turn on the backlight
    lcd.print("Pump: " + String(pumpStatus ? "ON" : "OFF"));
  }

  sendToThingSpeak(v0, v1, pumpStatus);
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


void update_screen() {
    lcd.clear();
    lcd.backlight(); // Turn on the backlight
    // First line
    lcd.setCursor(0, 0);
    lcd.print("WP1:" + String(wp0) + " TH1:" + String(field1Value));
    
    // Second line
    lcd.setCursor(0, 1);
    lcd.print("WP2:" + String(wp1) + " TH2:" + String(field2Value));
    
    // Third line
    lcd.setCursor(0, 2);
    lcd.print("Valves V0:" + String(v0 ? "ON" : "OFF") + " V1:" + String(v1 ? "ON" : "OFF"));

    // Fourth line
    lcd.setCursor(0, 3);
     lcd.print("Pump:" + String(pumpStatus ? "ON" : "OFF"));
      lcd.setCursor(9, 3);
     lcd.print("-LastStatus");
     delay(15000);
      lcd.clear();
    lcd.backlight(); // Turn on the backlight
          lcd.setCursor(0, 0);
     lcd.print("Going to Sleep..");
         lcd.setCursor(0, 1);
     lcd.print("See You Soon..");
          delay(5000);
     lcd.noBacklight(); // Turn off the backlight
  //  lcd.print("Pump: " + String(pumpStatus ? "ON" : "OFF") + " BAT: " + String(v_batt) + " PSU: " + String(v_psu));
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
