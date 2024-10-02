// Motor A Pins
const int motor1Pin1 = 27;
const int motor1Pin2 = 26;
const int enable1Pin = 14;

// PWM Properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

// Timing variables for non-blocking delays
unsigned long prevMillisForward = 0;
unsigned long prevMillisBackward = 0;
const unsigned long motorDelay = 2000; // Motor run time
const unsigned long stopDelay = 1000; // Motor stop time

// Error handling flag
bool motorError = false;

void setup() {
  // Set motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // Configure PWM for motor speed control
  ledcAttach(enable1Pin, freq, resolution); // Attach and set up the PWM

  Serial.begin(115200);
  Serial.println("Testing DC Motor...");
}

void loop() {
  unsigned long currentMillis = millis();

  // Check for errors
  if (motorError) {
    Serial.println("Motor error detected. Stopping motor.");
    stopMotor();
    return; // Exit the loop to prevent further execution
  }

  // Move the motor forward
  if (currentMillis - prevMillisForward >= motorDelay) {
    Serial.println("Moving Forward at max speed");
    controlMotor(true); // Move forward
    prevMillisForward = currentMillis;
  }

  // Stop the motor
  if (currentMillis - prevMillisForward >= motorDelay + stopDelay) {
    Serial.println("Motor stopped");
    stopMotor();
    prevMillisForward = currentMillis;
  }

  // Move the motor backward
  if (currentMillis - prevMillisBackward >= motorDelay) {
    Serial.println("Moving Backwards at max speed");
    controlMotor(false); // Move backward
    prevMillisBackward = currentMillis;
  }

  // Stop the motor after backward run
  if (currentMillis - prevMillisBackward >= motorDelay + stopDelay) {
    Serial.println("Motor stopped");
    stopMotor();
    prevMillisBackward = currentMillis;
  }

  // Gradually increase speed forward
  if (dutyCycle <= 255) {
    Serial.print("Increasing speed forward with duty cycle: ");
    Serial.println(dutyCycle);
    gradualSpeedUp();
  }
}

// Function to control motor direction
void controlMotor(bool forward) {
  digitalWrite(enable1Pin, HIGH); // Enable the motor driver
  if (forward) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  } else {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
  }
}

// Function to stop the motor
void stopMotor() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(enable1Pin, LOW); // Disable the motor driver
}

// Function to gradually increase motor speed
void gradualSpeedUp() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(enable1Pin, dutyCycle); // Use enable1Pin for PWM control
  dutyCycle += 5;

  // Add a delay to prevent excessive current draw during rapid speed changes
  delay(10); // Adjust the delay as needed
}
