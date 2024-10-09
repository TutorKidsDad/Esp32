#include <Wire.h>
#include <LiquidCrystal_I2C.h>   // Library for I2C LCD
#include <Encoder.h>             // Library for Rotary Encoder

// Stepper motor pins
const int DIR = 12;
const int STEP = 14;
const int steps_per_rev = 200;

// Rotary encoder pins
const int ENCODER_PIN_A = 2;     // Rotary encoder A pin
const int ENCODER_PIN_B = 3;     // Rotary encoder B pin
const int BUTTON_PIN = 4;        // Rotary encoder button pin (or separate button)

// Initialize Rotary Encoder (using Encoder library)
Encoder myEnc(ENCODER_PIN_A, ENCODER_PIN_B);

// Initialize I2C LCD (address: 0x27, 16 chars, 2 lines)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables to track motor control
long encoderPosition = 0;   // Tracks the position of the encoder
long newPosition = 0;       // New position from encoder
long speedDelay = 2000;     // Speed based on encoder (delay in microseconds)
bool direction = HIGH;      // Motor direction: HIGH = Clockwise, LOW = Counter-Clockwise

// Button debouncing variables
bool buttonState = HIGH;       // Current state of the button
bool lastButtonState = HIGH;   // Previous state of the button
unsigned long lastDebounceTime = 0;  // Last time the button was toggled
unsigned long debounceDelay = 50;    // Debounce delay (in ms)

void setup() {
  Serial.begin(115200);

  // Initialize stepper motor pins
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  // Initialize button pin
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Using internal pull-up resistor

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Step Motor Ctrl");
  lcd.setCursor(0, 1);
  lcd.print("Speed: 500us");
}

void loop() {
  // Read the current position of the encoder
  newPosition = myEnc.read();
  
  // If encoder position changed, update speed or direction
  if (newPosition != encoderPosition) {
    if (newPosition > encoderPosition) {
      speedDelay -= 100;  // Increase speed (decrease delay)
      if (speedDelay < 200) speedDelay = 200;  // Set a minimum speed limit
    } else {
      speedDelay += 100;  // Decrease speed (increase delay)
      if (speedDelay > 5000) speedDelay = 5000;  // Set a maximum speed limit
    }
    encoderPosition = newPosition;

    // Update the LCD display with speed and direction
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Speed: ");
    lcd.print(speedDelay);
    lcd.print(" us");

    lcd.setCursor(0, 1);
    lcd.print("Dir: ");
    lcd.print(direction == HIGH ? "Clockwise" : "Counter-Clk");
  }

  // Button debouncing for direction change
  int reading = digitalRead(BUTTON_PIN);

  // If the button state changes (pressed or released)
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Reset debounce timer
  }

  // If the current time - last debounce time is greater than the debounce delay, change direction
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Only toggle the direction if the button was pressed (state goes LOW)
    if (reading == LOW && buttonState == HIGH) {
      direction = !direction;  // Toggle direction
      lcd.setCursor(0, 1);
      lcd.print("Dir: ");
      lcd.print(direction == HIGH ? "Clockwise" : "Counter-Clk");
    }
    buttonState = reading;  // Update the button state
  }

  lastButtonState = reading;  // Update the last button state

  // Control stepper motor
  digitalWrite(DIR, direction);
  
  // Spin the motor
  digitalWrite(STEP, HIGH);
  delayMicroseconds(speedDelay);
  digitalWrite(STEP, LOW);
  delayMicroseconds(speedDelay);
}
