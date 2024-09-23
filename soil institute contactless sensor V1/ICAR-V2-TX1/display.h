#ifndef DISPLAY_H
#define DISPLAY_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration
#define LCD_ADDR 0x27  // I2C address of the LCD
#define LCD_COLUMNS 20 // Number of columns in the LCD
#define LCD_ROWS 4    // Number of rows in the LCD

// Initialize the LCD with the I2C address and dimensions
//LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLUMNS, LCD_ROWS);

int active_screen = 0;

// attach toggle switch with interrupts for switching the display modes 
void IRAM_ATTR switch_screen()
{
  active_screen = (active_screen + 1) % 2;
}

void init_display()
{
lcd.init(); // Initialize the LCD
lcd.backlight(); // Turn on the backlight
  lcd.clear(); // Clear the display
  lcd.setCursor(0, 0); // Set the cursor to the first row
  lcd.print(DEV_NAME); // Display the device name
      lcd.setCursor(0, 1);
  lcd.print("Checking WiFi..."); // Assuming DEV_NAME is defined el
    lcd.setCursor(0, 2);
  lcd.print("Please Wait.."); // Assuming DEV_NAME is defined el
      lcd.setCursor(0, 3);
  lcd.print("Processing the Data.."); // Assuming DEV_NAME is defined el
  delay(5000);
   lcd.noBacklight(); // Turn off the backlight
}

void update_screen()
{
  lcd.clear(); // Clear the display
lcd.backlight(); // Turn on the backlight
  if (active_screen == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("WP: ");
    lcd.print(wp);

    lcd.setCursor(0, 1);
    lcd.print("TMP: ");
    lcd.print(temp);
    lcd.print("C");

    // Additional lines if necessary
    lcd.setCursor(0, 2);
    lcd.print("RH: ");
    lcd.print(humidity);

    lcd.setCursor(0, 3);
    lcd.print("strip: ");
    lcd.print(strip);
   // lcd.print("%");
    delay(10000);
    lcd.clear(); // Clear the display
    lcd.setCursor(0, 0); // Set the cursor to the first row
    lcd.print("Going to deep sleep for 3 minutes");
     delay(15000);
     lcd.noBacklight(); // Turn off the backlight
  }
  else if (active_screen == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("STRIP: ");
    lcd.print(strip);

    // Add additional information or icons as needed
  }
}

#endif
