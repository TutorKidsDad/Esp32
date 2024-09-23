# LoRa Controlled Motor Vehicle with Joysticks

This project provides a wireless control system for a vehicle with four motors using LoRa communication and three joysticks. The system consists of two parts:
1. **Joystick Controller (Sender)**: Reads values from three joysticks to control speed and direction of four motors and sends the data over LoRa.
2. **Motor Vehicle (Receiver)**: Receives the control data via LoRa and adjusts the motors' speed and direction accordingly.

## Hardware Requirements

### For Joystick Controller (Sender)
- 3 Joysticks
- ESP32 or Arduino board with LoRa support
- LoRa module (e.g., SX1278, 915MHz frequency)
- Power supply

### For Motor Vehicle (Receiver)
- 4 Motors (2 controlled together, 2 controlled independently)
- ESP32 or Arduino board with LoRa support
- LoRa module (e.g., SX1278, 915MHz frequency)
- Motor driver (e.g., L298N or similar)
- Power supply for motors
- PWM-enabled pins for motor speed control

## Pin Configuration

### Joystick Controller (Sender)
- **Joystick 1** (Controls Motor 1 & 2):
  - X-Axis (Direction): Pin 34
  - Y-Axis (Speed): Pin 35
- **Joystick 2** (Controls Motor 3):
  - X-Axis (Direction): Pin 32
  - Y-Axis (Speed): Pin 33
- **Joystick 3** (Controls Motor 4):
  - X-Axis (Direction): Pin 36
  - Y-Axis (Speed): Pin 39

### Motor Vehicle (Receiver)
#### Motor 1 & 2 (Controlled Together)
- Motor 1 Forward: Pin 5
- Motor 1 Backward: Pin 18
- Motor 2 Forward: Pin 19
- Motor 2 Backward: Pin 21
- **PWM Speed Control**: Pin 22

#### Motor 3 (Controlled Independently)
- Motor 3 Forward: Pin 12
- Motor 3 Backward: Pin 13
- **PWM Speed Control**: Pin 27

#### Motor 4 (Controlled Independently)
- Motor 4 Forward: Pin 25
- Motor 4 Backward: Pin 26
- **PWM Speed Control**: Pin 14

## Installation

### 1. Joystick Controller (Sender)
1. Connect the LoRa module and joysticks to the ESP32 or Arduino.
2. Upload the joystick controller code to the microcontroller using Arduino IDE.
3. Ensure the correct frequency is set for LoRa communication (915 MHz in the provided example).

### 2. Motor Vehicle (Receiver)
1. Connect the LoRa module, motor driver, and motors to the ESP32 or Arduino.
2. Upload the motor control code to the microcontroller using Arduino IDE.
3. Ensure the same LoRa frequency is used for communication.

## Code Explanation

### 1. Joystick Controller (Sender)
- The controller reads analog values from three joysticks.
- It maps these values to motor speed (0-255) and determines the direction (forward/backward).
- The motor speeds and directions are then transmitted as a comma-separated packet over LoRa.
- **Data format**: 
motorSpeed1,motorDirection1_2,motorSpeed2,motorSpeed3,motorDirection3,motorSpeed4,motorDirection4

### 2. Motor Vehicle (Receiver)
- The receiver listens for LoRa packets and parses the motor speed and direction data.
- Based on the received data, it controls four motors: two motors (1 & 2) are controlled together, while motors 3 and 4 are controlled independently.
- The motors are driven forward or backward based on the direction value, and the speed is adjusted via PWM.

## Example LoRa Packet

150,1,130,120,-1,140,1


- **Motor 1**: Speed = 150, Direction = Forward
- **Motor 2**: Speed = 130, Direction = Forward
- **Motor 3**: Speed = 120, Direction = Backward
- **Motor 4**: Speed = 140, Direction = Forward

## Serial Debugging

### On the Motor Vehicle (Receiver)
The received motor speeds and directions are printed to the serial monitor for debugging purposes. For example:

Motor 1: Speed = 150, Direction = 1 Motor 2: Speed = 130 Motor 3: Speed = 120, Direction = -1 Motor 4: Speed = 140, Direction = 1


## Dependencies

- [LoRa Library](https://github.com/sandeepmistry/arduino-LoRa) for Arduino

Install the library using the Arduino Library Manager.

## License

This project is licensed under the MIT License.

