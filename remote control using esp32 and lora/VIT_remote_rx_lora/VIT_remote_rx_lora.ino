#include <LoRa.h>

// Motor 1 & 2 pins (controlled together for a vehicle)
int motor1Pin1 = 5;    // Motor 1 forward
int motor1Pin2 = 18;   // Motor 1 backward
int motor2Pin1 = 19;   // Motor 2 forward
int motor2Pin2 = 21;   // Motor 2 backward
int enableMotor1_2 = 22; // Speed (PWM) for Motors 1 & 2

// Motor 3 pins (controlled independently)
int motor3Pin1 = 12;   // Motor 3 forward
int motor3Pin2 = 13;   // Motor 3 backward
int enableMotor3 = 27; // Speed (PWM) for Motor 3

// Motor 4 pins (controlled independently)
int motor4Pin1 = 25;   // Motor 4 forward
int motor4Pin2 = 26;   // Motor 4 backward
int enableMotor4 = 14; // Speed (PWM) for Motor 4

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);  // Set frequency to match the transmitter
  
  // Set motor pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enableMotor1_2, OUTPUT);
  
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  
  pinMode(motor3Pin1, OUTPUT);
  pinMode(motor3Pin2, OUTPUT);
  pinMode(enableMotor3, OUTPUT);
  
  pinMode(motor4Pin1, OUTPUT);
  pinMode(motor4Pin2, OUTPUT);
  pinMode(enableMotor4, OUTPUT);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Receive data as a string (format: speed1,dir1,speed2,speed3,dir3,speed4,dir4)
    String receivedData = LoRa.readString();

    // Parse the received values
    int firstDelimiter = receivedData.indexOf(',');
    int secondDelimiter = receivedData.indexOf(',', firstDelimiter + 1);
    int thirdDelimiter = receivedData.indexOf(',', secondDelimiter + 1);
    int fourthDelimiter = receivedData.indexOf(',', thirdDelimiter + 1);
    int fifthDelimiter = receivedData.indexOf(',', fourthDelimiter + 1);
    int sixthDelimiter = receivedData.indexOf(',', fifthDelimiter + 1);
    int seventhDelimiter = receivedData.indexOf(',', sixthDelimiter + 1);
    
    int motorSpeed1 = receivedData.substring(0, firstDelimiter).toInt();
    int motorDirection1_2 = receivedData.substring(firstDelimiter + 1, secondDelimiter).toInt();
    int motorSpeed2 = receivedData.substring(secondDelimiter + 1, thirdDelimiter).toInt();
    
    int motorSpeed3 = receivedData.substring(thirdDelimiter + 1, fourthDelimiter).toInt();
    int motorDirection3 = receivedData.substring(fourthDelimiter + 1, fifthDelimiter).toInt();
    
    int motorSpeed4 = receivedData.substring(fifthDelimiter + 1, sixthDelimiter).toInt();
    int motorDirection4 = receivedData.substring(sixthDelimiter + 1).toInt();

    // Control Motor 1
    analogWrite(enableMotor1_2, motorSpeed1);  // Speed control for Motor 1 & 2
    if (motorDirection1_2 == 1) {
      digitalWrite(motor1Pin1, HIGH);
      digitalWrite(motor1Pin2, LOW);
    } else {
      digitalWrite(motor1Pin1, LOW);
      digitalWrite(motor1Pin2, HIGH);
    }

    // Control Motor 2
    if (motorDirection1_2 == 1) {
      digitalWrite(motor2Pin1, HIGH);
      digitalWrite(motor2Pin2, LOW);
    } else {
      digitalWrite(motor2Pin1, LOW);
      digitalWrite(motor2Pin2, HIGH);
    }

    // Control Motor 3
    analogWrite(enableMotor3, motorSpeed3);
    if (motorDirection3 == 1) {
      digitalWrite(motor3Pin1, HIGH);
      digitalWrite(motor3Pin2, LOW);
    } else {
      digitalWrite(motor3Pin1, LOW);
      digitalWrite(motor3Pin2, HIGH);
    }

    // Control Motor 4
    analogWrite(enableMotor4, motorSpeed4);
    if (motorDirection4 == 1) {
      digitalWrite(motor4Pin1, HIGH);
      digitalWrite(motor4Pin2, LOW);
    } else {
      digitalWrite(motor4Pin1, LOW);
      digitalWrite(motor4Pin2, HIGH);
    }

    // Debugging output
    Serial.print("Motor 1: Speed = ");
    Serial.print(motorSpeed1);
    Serial.print(", Direction = ");
    Serial.println(motorDirection1_2);
    
    Serial.print("Motor 2: Speed = ");
    Serial.println(motorSpeed2);

    Serial.print("Motor 3: Speed = ");
    Serial.print(motorSpeed3);
    Serial.print(", Direction = ");
    Serial.println(motorDirection3);
    
    Serial.print("Motor 4: Speed = ");
    Serial.print(motorSpeed4);
    Serial.print(", Direction = ");
    Serial.println(motorDirection4);
  }
}
