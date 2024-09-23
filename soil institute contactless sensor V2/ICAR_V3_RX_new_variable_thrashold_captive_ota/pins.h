#ifndef PINS_H
#define PINS_H

//#define OLED_ADDR     0x3C  
#define LCD_ADDR 0x27  // I2C address of the LCD
#define SCL 22
#define SDA 21

#define R_LED 4
#define G_LED 18
#define B_LED 19

#define V_BATT 34
#define V_PSU 39

//#define BUZZER 26
// Define the pins for wifi
#define PIN1 15  // D15 on ESP32
#define PIN2 2   // D2 on ESP32

//-------------- Relays --------------
#define VALVE0 13
#define VALVE1 26
//#define VALVE2 4
//#define VALVE3 16

//-------------- PUMP --------------
#define PUMP 27

#define PWR_SW 17

//------------- Water Level Sensor -------
#define WL_SENSE0 35
#define WL_SENSE1 32

#endif
