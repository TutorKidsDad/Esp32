#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "sensors.h"

// LCD dimensions
#define LCD_WIDTH 16
#define LCD_HEIGHT 2

// Create an LCD object with I2C address 0x27 (adjust if necessary)
LiquidCrystal_I2C lcd(0x27, LCD_WIDTH, LCD_HEIGHT);

int active_screen = 0;

// Function declarations
void switch_screen();
void init_display();
void update_screen();
#include "display.h"

void switch_screen()
{
  active_screen = (active_screen + 1) % 5;
  update_screen();
}

void init_display()
{
lcd.init(); // Initialize the LCD
lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print(DEV_NAME); // Assuming DEV_NAME is defined elsewhere
  delay(2000); // Pause to let the user see the initial message
}

void update_screen()
{
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor to first line

  switch (active_screen)
  {
    case 0:
      lcd.print("TX0 Data");
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print("WP: " + String(wp0) + " H: " + String(humidity0));
      break;

    case 1:
      lcd.print("TX1 Data");
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print("WP: " + String(wp1) + " H: " + String(humidity1));
      break;

    case 2:
      lcd.print("Valve Status");
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print("V0: " + String(v0 ? "ON" : "OFF") + " V1: " + String(v1 ? "ON" : "OFF"));
      break;

    case 3:
      lcd.print("System Status");
      lcd.setCursor(0, 1); // Set cursor to third line (assuming 20x4 LCD or use a second line if 16x2)
      lcd.print("Pump: " + String(pumpStatus ? "ON" : "OFF"));
      break;
      
/*      
    case 2:
      lcd.print("TX2 Data");
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print("WP: " + String(wp2) + " H: " + String(humidity2));
      break;

    case 3:
      lcd.print("TX3 Data");
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print("WP: " + String(wp3) + " H: " + String(humidity3));
      break;
*/
    case 4:
      lcd.print("MEGH-RX");
      lcd.setCursor(0, 1); // Set cursor to second line
      lcd.print("BAT: " + String(v_batt) + " PSU: " + String(v_psu));
      break;
  }
}

#endif
