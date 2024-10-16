// inspired by https://www.instructables.com/Arduino-RGB-Color-Picker-Pick-Colors-From-Real-Lif/
#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <LiquidCrystal_I2C.h>  // Include the library for I2C LCD

// Initialize I2C LCD (address 0x27 is commonly used for 16x2 LCDs)
LiquidCrystal_I2C lcd(0x27, 16, 2);  

byte gammatable[256];  // Table to convert measured RGB values for realistic visualization

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // Color sensor

// Define ESP32 GPIO pins
int pinLED = 15;      // White LED pin (or sensor LED)
int pinButton = 14;   // Button pin
int redLED = 16;      // Red LED pin
int greenLED = 17;    // Green LED pin
int blueLED = 18;     // Blue LED pin

#define commonAnode false  // Common cathode RGB LED

void setup() {
  // Initialize pins
  pinMode(pinLED, OUTPUT);
  pinMode(pinButton, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(blueLED, OUTPUT);
  
  // Initialize I2C LCD
  lcd.init();
  lcd.backlight(); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Colour Picker");

  // Initialize the color sensor
  if (tcs.begin()) {
    lcd.setCursor(0, 1);
    lcd.print("Ready");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("Sensor Error");
    while (1);
  }

  // Set the RGB LED to fade
  analogWrite(redLED, 255);
  for (int i = 0; i <= 255; i++) {
    analogWrite(redLED, 255 - i);
    analogWrite(greenLED, i);
    delay(3);
  }
  for (int i = 0; i <= 255; i++) {
    analogWrite(greenLED, 255 - i);
    analogWrite(blueLED, i);
    delay(3);
  }
  analogWrite(blueLED, 0);

  digitalWrite(pinLED, LOW); // Turn off the sensor's LED

  // Setup the gamma table for RGB conversion
  for (int i = 0; i < 256; i++) {
    float x = i / 255.0;
    x = pow(x, 2.5) * 255;
    gammatable[i] = commonAnode ? 255 - x : x;
  }
}

void loop() {
  if (digitalRead(pinButton) == LOW) {  // If button is pressed
    float red, green, blue;

    // Turn on the sensor's LED for color measurement
    digitalWrite(pinLED, HIGH);
    delay(500);
    tcs.setInterrupt(false);  // Start measurement
    delay(60);  // Delay for measurement
    tcs.getRGB(&red, &green, &blue);  // Get RGB values
    tcs.setInterrupt(true);  // Stop measurement
    delay(500);  // Avoid repeated readings
    digitalWrite(pinLED, LOW);  // Turn off sensor LED

    // Display RGB values on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Colour Picked");
    lcd.setCursor(0, 1);
    lcd.print("R:");
    lcd.print(int(red));
    lcd.print(" G:");
    lcd.print(int(green));
    lcd.print(" B:");
    lcd.print(int(blue));

    // Set the RGB LED to reflect the measured color
    analogWrite(redLED, gammatable[int(red)]);
    analogWrite(greenLED, gammatable[int(green)]);
    analogWrite(blueLED, gammatable[int(blue)]);

    delay(500);
  }
}
