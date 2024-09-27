#include <BTS7960.h>
#include <Arduino.h>

// Motor 1 control pins (BTS7960B H-Bridge Motor Driver 1)
const int RPWM_Motor1_Pin = 26;  // Right motor PWM (forward)
const int LPWM_Motor1_Pin = 25;  // Left motor PWM (backward)
const int R_EN_Motor1_Pin = 33;  // Enable right motor (forward)
const int L_EN_Motor1_Pin = 32;  // Enable left motor (backward)
const int R_IS_Motor1_Pin = 35;  // Right Current Sense for Motor 1
const int L_IS_Motor1_Pin = 34;  // Left Current Sense for Motor 1

// Motor 2 control pins (BTS7960B H-Bridge Motor Driver 2)
const int RPWM_Motor2_Pin = 19;  // Right motor PWM (forward)
const int LPWM_Motor2_Pin = 21;  // Left motor PWM (backward)
const int R_EN_Motor2_Pin = 22;  // Enable right motor (forward)
const int L_EN_Motor2_Pin = 23;  // Enable left motor (backward)
const int R_IS_Motor2_Pin = 36;  // Right Current Sense for Motor 2 VP
const int L_IS_Motor2_Pin = 39;  // Left Current Sense for Motor 2 V

// Additional motor control pin (speed only)
const int AdditionalMotor_Pin = 14;  // PWM pin for the additional motor
const int LPWM_Motor3_Pin = 12;   // PWM pin for controlling motor speed (Left PWM)

const int R_EN_Motor3_Pin = 16;  // Enable right motor (forward)rx2
const int L_EN_Motor3_Pin = 17;  // Enable right motor (forward)rx2
//const int R_IS_Motor3_Pin = 17;  // Right Current Sense for Motor 3 tx2

// Remote control pins (input from drone remote)
const int Throttle_Pin = 18;  // Throttle input (channel 1)
const int Yaw_Pin = 4;       // Yaw input (channel 2)
const int SpeedControl_Pin = 27; // Receiver Channel for additional motor speed control

// Create BTS7960 motor objects (if library provides this)
BTS7960 motor1(L_EN_Motor1_Pin, R_EN_Motor1_Pin, LPWM_Motor1_Pin, RPWM_Motor1_Pin);
BTS7960 motor2(L_EN_Motor2_Pin, R_EN_Motor2_Pin, LPWM_Motor2_Pin, RPWM_Motor2_Pin);
BTS7960 motor3(L_EN_Motor3_Pin, R_EN_Motor3_Pin, LPWM_Motor3_Pin, AdditionalMotor_Pin);

void setup() {
  Serial.begin(115200);  // Start serial communication

  // Initialize motor drivers
  motor1.begin();
  motor1.enable();
  
  motor2.begin();
  motor2.enable();

  motor3.begin();
  motor3.enable();
  
  pinMode(AdditionalMotor_Pin, OUTPUT);  // Set additional motor control pin as output
    pinMode(LPWM_Motor3_Pin, OUTPUT);   // Set Left PWM pin as output

  // Set up the throttle and yaw pins as inputs
  pinMode(Throttle_Pin, INPUT);
  pinMode(Yaw_Pin, INPUT);
  pinMode(SpeedControl_Pin, INPUT);
    pinMode(R_EN_Motor3_Pin, OUTPUT);    // Set Right Enable pin as output
  pinMode(L_EN_Motor3_Pin, OUTPUT);    // Set Left Enable pin as output

  // Set motor control pins as outputs
  pinMode(RPWM_Motor1_Pin, OUTPUT);
  pinMode(LPWM_Motor1_Pin, OUTPUT);
  pinMode(RPWM_Motor2_Pin, OUTPUT);
  pinMode(LPWM_Motor2_Pin, OUTPUT);

  
  // Enable both motors
  digitalWrite(R_EN_Motor3_Pin, HIGH);  // Enable right motor
  digitalWrite(L_EN_Motor3_Pin, HIGH);  // Enable left motor
}

void loop() {
  // Read the throttle and yaw signals from the drone remote (PWM signals)
  int throttleValue = pulseIn(Throttle_Pin, HIGH, 25000);  // Throttle input
  int yawValue = pulseIn(Yaw_Pin, HIGH, 25000);            // Yaw input
  int speedControlValue = pulseIn(SpeedControl_Pin, HIGH, 25000);  // Additional motor speed control


    // Failsafe: stop all motors if no signal is received
  if (throttleValue == 0 || yawValue == 0) {
    stopAllMotors();
    Serial.println("No signal from transmitter, stopping all motors.");
    return;
  }
  
  // Debugging: Print received values
  Serial.print("Throttle: ");
  Serial.print(throttleValue);
  Serial.print(" | Yaw: ");
  Serial.println(yawValue);
Serial.print(" | PumpMotor: ");
  Serial.println(speedControlValue);

  //checkFaults();
  
  // Control motors based on throttle and yaw values
  controlMotors(throttleValue, yawValue);
  
 // Control the additional motor's speed
  controlAdditionalMotor(speedControlValue);
  delay(100);  // Small delay to avoid overloading
}


// Function to stop all motors
void stopAllMotors() {
  analogWrite(LPWM_Motor1_Pin, 0);
  analogWrite(RPWM_Motor1_Pin, 0);
  analogWrite(LPWM_Motor2_Pin, 0);
  analogWrite(RPWM_Motor2_Pin, 0);
  analogWrite(AdditionalMotor_Pin, 0);
}

// Function to control the motors based on throttle and yaw
void controlMotors(int throttle, int yaw) {
  // Map throttle to motor speed (from 0 to 255 based on the remote's PWM signal)
  int speed = 0;

  // Throttle control: forward and backward
  if (throttle > 1540) {  // Throttle forward (progressive speed)
    speed = map(throttle, 1600, 1950, 0, 255);  // Map throttle from 1540 to 1950 to speed range 0 to 255
    analogWrite(LPWM_Motor1_Pin, 0);  // Stop Motor1 backward
    analogWrite(RPWM_Motor1_Pin, speed);  // Motor1 forward

    analogWrite(LPWM_Motor2_Pin, 0);  // Stop Motor2 backward
    analogWrite(RPWM_Motor2_Pin, speed);  // Motor2 forward

    Serial.print("Forward | Speed: ");
    Serial.println(speed);
  } else if (throttle < 1490) {  // Throttle backward (progressive speed)
    speed = map(throttle, 1150, 1450, 255, 0);  // Map throttle from 1150 to 1490 to speed range 255 to 0 (reversed)
    analogWrite(RPWM_Motor1_Pin, 0);  // Stop Motor1 forward
    analogWrite(LPWM_Motor1_Pin, speed);  // Motor1 backward

    analogWrite(RPWM_Motor2_Pin, 0);  // Stop Motor2 forward
    analogWrite(LPWM_Motor2_Pin, speed);  // Motor2 backward

    Serial.print("Backward | Speed: ");
    Serial.println(speed);
  } else {  // Throttle in dead zone, allow yaw control
    // Stop both motors initially
    analogWrite(LPWM_Motor1_Pin, 0);
    analogWrite(RPWM_Motor1_Pin, 0);
    analogWrite(LPWM_Motor2_Pin, 0);
    analogWrite(RPWM_Motor2_Pin, 0);

    // Yaw control: turning in place by moving motors in opposite directions
    if (yaw > 1575) {  // Turn right (progressively increase turn speed)
      int turnSpeed = map(yaw, 1600, 1930, 0, 255);  // Map yaw from 1575 to 1930 to turn speed 0 to 255
      analogWrite(LPWM_Motor1_Pin, turnSpeed);  // Motor1 backward (turn right)
      analogWrite(RPWM_Motor2_Pin, turnSpeed);  // Motor2 forward (turn right)

      Serial.print("Turning Right | Turn Speed: ");
      Serial.println(turnSpeed);
    } else if (yaw < 1525) {  // Turn left (progressively increase turn speed)
      int turnSpeed = map(yaw, 1125, 1500, 255, 0);  // Map yaw from 1125 to 1525 to turn speed 255 to 0
      analogWrite(RPWM_Motor1_Pin, turnSpeed);  // Motor1 forward (turn left)
      analogWrite(LPWM_Motor2_Pin, turnSpeed);  // Motor2 backward (turn left)

      Serial.print("Turning Left | Turn Speed: ");
      Serial.println(turnSpeed);
    }
  }
}


// Function to check motor driver faults (Right IS and Left IS)
void checkFaults() {
  if (digitalRead(R_IS_Motor1_Pin) == LOW) {
    Serial.println("Fault detected on Motor 1 Right side");
    motor1.disable();  // Disable motor1 to clear fault
    delay(100);
    motor1.enable();   // Re-enable motor1 after clearing fault
  }
  
  if (digitalRead(L_IS_Motor1_Pin) == LOW) {
    Serial.println("Fault detected on Motor 1 Left side");
    motor1.disable();  // Disable motor1 to clear fault
    delay(100);
    motor1.enable();   // Re-enable motor1 after clearing fault
  }

  if (digitalRead(R_IS_Motor2_Pin) == LOW) {
    Serial.println("Fault detected on Motor 2 Right side");
    motor2.disable();  // Disable motor2 to clear fault
    delay(100);
    motor2.enable();   // Re-enable motor2 after clearing fault
  }

  if (digitalRead(L_IS_Motor2_Pin) == LOW) {
    Serial.println("Fault detected on Motor 2 Left side");
    motor2.disable();  // Disable motor2 to clear fault
    delay(100);
    motor2.enable();   // Re-enable motor2 after clearing fault
  }
/*
    if (digitalRead(R_IS_Motor3_Pin) == LOW) {
    Serial.println("Fault detected on Motor 3 Right side");
    motor3.disable();  // Disable motor3 to clear fault
    delay(100);
    motor3.enable();   // Re-enable motor3 after clearing fault
  }*/
}
// Function to control the additional motor's speed
void controlAdditionalMotor(int speedControl) {
  // Map speed control to PWM value (0 to 255)
  int mappedSpeed = map(speedControl, 1050, 2066, 0, 255); // Adjust range as necessary
  mappedSpeed = constrain(mappedSpeed, 0, 255);  // Constrain the mapped value to the range 0-255
  
  analogWrite(AdditionalMotor_Pin, mappedSpeed);  // Control additional motor speed
  Serial.print("Additional Motor Speed Control Value: ");
  Serial.println(mappedSpeed);
}

/*
// Function to control the additional motor's speed based on SpeedControl_Pin
void controlAdditionalMotor(int speedControlValue) {
  // Read the speed control signal (PWM signal)
  int speedControlValue = pulseIn(SpeedControl_Pin, HIGH, 25000);  // Read PWM signal (max duration 25ms)

  // Map the speed control value to the motor's speed range (0 to 255)
  int speed = map(speedControlValue, 1000, 2000, 0, 255); // Adjust the range as needed
  speed = constrain(speed, 0, 255);  // Ensure speed is within valid range

  // Control motor direction based on speed
  if (speed > 0) {
    analogWrite(AdditionalMotor_Pin, speed);  // Set right motor speed
  / analogWrite(LPWM_Motor3_Pin, 0);       // Set left motor speed (off)
    Serial.print("Motor running forward at speed: ");
    Serial.println(speed);
  } else {
    // Stop motors if speed is 0
    analogWrite(AdditionalMotor_Pin, 0);      // Stop right motor
   analogWrite(LPWM_Motor3_Pin, 0);      // Stop left motor
    Serial.println("Motor stopped");
  }

  delay(100);  // Small delay to avoid overloading
}*/
