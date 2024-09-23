# LoRa Controlled Motor Vehicle with Joystick

This project controls a vehicle with four motors using LoRa communication and three joysticks. Each joystick controls the speed and direction of a pair of motors, with the data being sent over LoRa for wireless control.

## Hardware Requirements

- 4 Motors
- 3 Joysticks
- ESP32 or Arduino board with LoRa support
- LoRa module (e.g., SX1278, 915MHz frequency)
- Motor driver (e.g., L298N or similar) for each motor
- Power supply for the motors
- PWM-enabled pins for motor speed control

## Pin Configuration

### Joystick 1 (Motor 1 & 2 Control)
- **X-Axis (Direction)**: Pin 34
- **Y-Axis (Speed)**: Pin 35

### Joystick 2 (Motor 3 Control)
- **X-Axis (Direction)**: Pin 32
- **Y-Axis (Speed)**: Pin 33

### Joystick 3 (Motor 4 Control)
- **X-Axis (Direction)**: Pin 36
- **Y-Axis (Speed)**: Pin 39

## Installation

1. Connect the LoRa module, joysticks, and motor driver to the ESP32 or Arduino.
2. Upload the provided code to the microcontroller using the Arduino IDE.
3. Adjust the LoRa frequency if needed to match your region.

## Code Explanation

- The code reads the analog values from the three joysticks.
- It then maps the values to motor speed (0-255) and direction (forward/backward).
- The mapped values are sent over LoRa to control the motors.

### Joystick Control

- **Joystick 1** controls **Motor 1 & 2**:
  - Y-axis controls speed.
  - X-axis adjusts the left-right turning by controlling the speed difference between Motor 1 and Motor 2.
- **Joystick 2** controls **Motor 3**:
  - Y-axis controls speed.
  - X-axis adjusts the direction.
- **Joystick 3** controls **Motor 4**:
  - Y-axis controls speed.
  - X-axis adjusts the direction.

### Data Format (Sent over LoRa)

The LoRa packet contains comma-separated values representing motor speeds and directions:

motorSpeed1,motorDirection1_2,motorSpeed2,motorSpeed3,motorDirection3,motorSpeed4,motorDirection4

### Example Packet

150,1,130,120,-1,140,1


- **Motor 1**: Speed = 150, Direction = Forward
- **Motor 2**: Speed = 130, Direction = Forward (since it's paired with Motor 1)
- **Motor 3**: Speed = 120, Direction = Backward
- **Motor 4**: Speed = 140, Direction = Forward

## Dependencies

- [LoRa Library](https://github.com/sandeepmistry/arduino-LoRa) for Arduino

Install the library using the Arduino Library Manager.

## License

This project is licensed under the MIT License.

