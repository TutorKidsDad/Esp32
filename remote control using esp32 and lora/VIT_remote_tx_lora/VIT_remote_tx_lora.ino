#include <LoRa.h>

// Joystick 1 (for Motor 1 & 2 - speed and direction)
int xAxisPin1 = 34;  // Joystick 1 X-axis pin (for direction control for Motor 1 & 2)
int yAxisPin1 = 35;  // Joystick 1 Y-axis pin (for speed control for Motor 1 & 2)

// Joystick 2 (for Motor 3 - speed and direction)
int xAxisPin2 = 32;  // Joystick 2 X-axis pin (for direction control for Motor 3)
int yAxisPin2 = 33;  // Joystick 2 Y-axis pin (for speed control for Motor 3)

// Joystick 3 (for Motor 4 - speed and direction)
int xAxisPin3 = 36;  // Joystick 3 X-axis pin (for direction control for Motor 4)
int yAxisPin3 = 39;  // Joystick 3 Y-axis pin (for speed control for Motor 4)

void setup() {
  Serial.begin(9600);
  LoRa.begin(915E6);  // Set frequency for LoRa (adjust for your region)
}

void loop() {
  // Reading joystick values for Motors 1 & 2
  int xValue1 = analogRead(xAxisPin1);  // X-axis for direction control for Motor 1 & 2 (left/right)
  int yValue1 = analogRead(yAxisPin1);  // Y-axis for speed control for Motor 1 & 2 (forward/backward)

  // Reading joystick values for Motor 3
  int xValue2 = analogRead(xAxisPin2);  // X-axis for direction control for Motor 3
  int yValue2 = analogRead(yAxisPin2);  // Y-axis for speed control for Motor 3

  // Reading joystick values for Motor 4
  int xValue3 = analogRead(xAxisPin3);  // X-axis for direction control for Motor 4
  int yValue3 = analogRead(yAxisPin3);  // Y-axis for speed control for Motor 4

  // Normalize values to be between 0 and 255 (speed) and determine direction
  int motorSpeed1_2 = map(abs(yValue1 - 2048), 0, 2048, 0, 255);  // Speed for Motor 1 & 2
  int motorDirection1_2 = (yValue1 > 2048) ? 1 : -1;  // Direction for Motor 1 & 2 (forward/backward)
  int turnOffset1_2 = map(xValue1, 0, 4095, -128, 128);  // Adjust for turning (X-axis)
  int motorSpeed1 = motorSpeed1_2 + turnOffset1_2;  // Adjust Motor 1 speed
  int motorSpeed2 = motorSpeed1_2 - turnOffset1_2;  // Adjust Motor 2 speed
  motorSpeed1 = constrain(motorSpeed1, 0, 255);
  motorSpeed2 = constrain(motorSpeed2, 0, 255);

  // Normalize values for Motor 3
  int motorSpeed3 = map(abs(yValue2 - 2048), 0, 2048, 0, 255);  // Speed for Motor 3
  int motorDirection3 = (yValue2 > 2048) ? 1 : -1;  // Direction for Motor 3
  int turnOffset3 = map(xValue2, 0, 4095, -128, 128);
  motorSpeed3 = constrain(motorSpeed3 + turnOffset3, 0, 255);  // Apply turn adjustment for Motor 3

  // Normalize values for Motor 4
  int motorSpeed4 = map(abs(yValue3 - 2048), 0, 2048, 0, 255);  // Speed for Motor 4
  int motorDirection4 = (yValue3 > 2048) ? 1 : -1;  // Direction for Motor 4
  int turnOffset4 = map(xValue3, 0, 4095, -128, 128);
  motorSpeed4 = constrain(motorSpeed4 + turnOffset4, 0, 255);  // Apply turn adjustment for Motor 4

  // Send control data over LoRa
  LoRa.beginPacket();
  LoRa.print(motorSpeed1);  // Speed of Motor 1
  LoRa.print(",");
  LoRa.print(motorDirection1_2);  // Direction of Motor 1 & 2
  LoRa.print(",");
  LoRa.print(motorSpeed2);  // Speed of Motor 2
  LoRa.print(",");
  LoRa.print(motorSpeed3);  // Speed of Motor 3
  LoRa.print(",");
  LoRa.print(motorDirection3);  // Direction of Motor 3
  LoRa.print(",");
  LoRa.print(motorSpeed4);  // Speed of Motor 4
  LoRa.print(",");
  LoRa.print(motorDirection4);  // Direction of Motor 4
  LoRa.endPacket();

  delay(100);  // Transmit every 100ms
}
