#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the address of the I2C LCD display (can vary depending on the hardware)
#define I2C_ADDR 0x27

// Set the number of columns and rows in the LCD display
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// Define the custom characters for the power plug and battery icons
byte batteryChar[8] ={
  B01110,
  B01110,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111
};
byte powerPlugChar[8] = {
  B01010,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00100
};

// Initialize the I2C LCD display library
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

void setup() {
  // Initialize the I2C communication with the LCD display
  Wire.begin();
  
  // Initialize the LCD display
  lcd.init();
  
  // Turn on the backlight
  lcd.backlight();
  
  // Load the custom characters for the power plug and battery icons
  lcd.createChar(0, powerPlugChar);
  lcd.createChar(1, batteryChar);
  
  // Display the power plug icon in the first row of the LCD display
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  
  // Display the battery icon in the second row of the LCD display
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
}

void loop() {
  // Do nothing in the loop function (the display is already set up)
}
