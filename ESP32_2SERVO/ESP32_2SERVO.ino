#include <ESP32Servo.h>

// Create servo objects for controlling two servos
Servo myservo1;
Servo myservo2;

// Pin assignments for the two servos
int servo1Pin = 13;
int servo2Pin = 12;

// Movement variables
int pos1 = 0;  // Position for servo 1
int pos2 = 0;  // Position for servo 2
bool increasing = true; // Direction of movement (used for both servos)

// Adjustable parameters
const int minAngle = 0;
const int maxAngle = 180;
const int sweepDelay = 15;   // Delay time for each step (milliseconds)
unsigned long lastMoveTime = 0;  // Variable to keep track of the last movement time

void setup() {
  // Allocate timers for PWM control
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Set PWM frequency to 50 Hz (standard for servos)
  myservo1.setPeriodHertz(50);
  myservo2.setPeriodHertz(50);

  // Attach servos to their respective pins, define pulse range for accurate movement
  myservo1.attach(servo1Pin, 500, 2400);
  myservo2.attach(servo2Pin, 500, 2400);
}

// Function to sweep the servos
void sweepServos() {
  if (increasing) {
    // Increase angle for both servos
    pos1++;
    pos2++;
    if (pos1 >= maxAngle) {
      increasing = false;
    }
  } else {
    // Decrease angle for both servos
    pos1--;
    pos2--;
    if (pos1 <= minAngle) {
      increasing = true;
    }
  }

  // Write the new positions to both servos
  myservo1.write(pos1);
  myservo2.write(pos2);
}

void loop() {
  // Check if enough time has passed for the next servo movement
  if (millis() - lastMoveTime >= sweepDelay) {
    sweepServos();           // Sweep the servos
    lastMoveTime = millis(); // Update the last movement time
  }
}
