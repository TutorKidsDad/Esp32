#ifndef PINS_H
#define PINS_H

#define TCS_ADDR      0x29
#define AHT20_ADDR    0x38  
//#define OLED_ADDR     0x3C  
#define LCD_ADDR 0x27  // I2C address of the LCD

#define SCL 22
#define SDA 21

#define R_LED  4  //27 ORIGINAL
#define G_LED 18 //13 ORIGINAL
#define B_LED 19 //12 ORIGINAL

#define STRIP_LED 14
#define MOISTURE_IN 34
#define V_BATT 32
#define BUZZER 12

// Define the pins for wifi
#define PIN1 15  // D15 on ESP32
#define PIN2 2   // D2 on ESP32
#endif
