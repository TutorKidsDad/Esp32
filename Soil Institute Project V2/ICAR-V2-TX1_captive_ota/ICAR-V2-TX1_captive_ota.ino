#define DEV_ID0 0
//#define DEV_ID1 1
//#define DEV_ID2 2
//#define DEV_ID3 3

#include "pins.h"
#include "comms.h"
#include "OTA.h"
#include "sensors.h"
#include "display.h"
#include <esp_sleep.h> // Include the ESP32 sleep library
#include <time.h>      // Include time library // CHANGE: Added time library

sensors_event_t h;
sensors_event_t t;
int prev_t;

void setup() 
{
       lcd.noBacklight(); // Turn off the backlight
  Serial.begin(BAUD_RATE);
    Serial.println("Tx0 Init");
    // Check why the ESP32 woke up
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
    Serial.println("Woke up from deep sleep");
  } else {
    Serial.println("Not a deep sleep wake-up");
  }
  init_pins();
 // setup_OTA();
  connectToWiFiOTA();
  checkForUpdate();
 // connectToWiFi();
  init_display();
      lcd.noBacklight(); // Turn off the backlight
  init_sensors();
 // connectToWiFi();
  digitalWrite(STRIP_LED, HIGH);
  delay(2000);
  digitalWrite(STRIP_LED, LOW);
  prev_t = millis();

  digitalWrite(R_LED, HIGH);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, LOW);
  delay(2000);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, HIGH);
  digitalWrite(B_LED, LOW);
  delay(2000);
  digitalWrite(R_LED, LOW);
  digitalWrite(G_LED, LOW);
  digitalWrite(B_LED, HIGH);

      // Initialize the pins as outputs
    pinMode(PIN1, OUTPUT);
    pinMode(PIN2, OUTPUT);

    // Start with one pin high and the other low
    digitalWrite(PIN1, HIGH);
    digitalWrite(PIN2, LOW);
}

void loop() 
{
 // ArduinoOTA.handle();
  read_data();
  update_screen();

 //  read_data();
//  upload_data();BUZZER

  
  if(millis() - prev_t > 5000)
  {
    upload_data(); 
    prev_t = millis();
  }
  if(vbatt < 3.5)
  {
    lcd.clear(); // Clear the display
    lcd.setCursor(0, 0); // Set the cursor to the first row
    lcd.print("Battery low");
    delay(3000);
    lcd.clear(); // Clear the display
    lcd.setCursor(0, 0); // Set the cursor to the first row
    lcd.print("Going to deep sleep for 3 minutes");
    //delay(3000);
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
    digitalWrite(B_LED, LOW);
   // while(1);
  }
  delay(3000);
  // Enter deep sleep for 3 minutes (180000 milliseconds)
  esp_sleep_enable_timer_wakeup(sleepDuration); // Time in microseconds
  Serial.println("Going to deep sleep for 3 minutes");
      lcd.noBacklight(); // Turn off the backlight
  esp_deep_sleep_start();
  delay(1000);
}
