LoRa Controlled Motor Vehicle

This project allows for remote control of a vehicle with four motors using LoRa communication. Two motors are controlled together for movement, while the other two are controlled independently.
Hardware Requirements

    4 Motors (2 controlled together, 2 independently)
    ESP32 or Arduino board with LoRa support
    LoRa module (e.g., SX1278, 915MHz frequency)
    Motor driver (e.g., L298N or similar)
    PWM-enabled pins for motor speed control
    Power supply for motors

Pin Configuration
Motor 1 & 2 (Controlled Together)

    Motor 1 Forward: Pin 5
    Motor 1 Backward: Pin 18
    Motor 2 Forward: Pin 19
    Motor 2 Backward: Pin 21
    PWM Speed Control: Pin 22

Motor 3 (Controlled Independently)

    Motor 3 Forward: Pin 12
    Motor 3 Backward: Pin 13
    PWM Speed Control: Pin 27

Motor 4 (Controlled Independently)

    Motor 4 Forward: Pin 25
    Motor 4 Backward: Pin 26
    PWM Speed Control: Pin 14

Installation

    Connect the LoRa module and motor driver to the microcontroller.
    Upload the provided code to the microcontroller using the Arduino IDE.
    Ensure the correct frequency (915 MHz in this case) is used for the LoRa module.

Code Explanation

    The code sets up a LoRa receiver to parse incoming packets containing motor control data.
    Data format: speed1,dir1,speed2,speed3,dir3,speed4,dir4
        speedX: Speed of motor X (0-255 for PWM control)
        dirX: Direction of motor X (1 for forward, 0 for backward)
    Motors 1 & 2 are controlled together, while motors 3 & 4 are controlled independently.

Example Packet

100,1,100,120,0,150,1

    Motor 1 & 2: Speed = 100, Direction = Forward
    Motor 3: Speed = 120, Direction = Backward
    Motor 4: Speed = 150, Direction = Forward

Serial Debugging

The code outputs the motor speeds and directions via the serial monitor for debugging.

yaml

Motor 1: Speed = 100, Direction = 1
Motor 2: Speed = 100
Motor 3: Speed = 120, Direction = 0
Motor 4: Speed = 150, Direction = 1

Dependencies

    LoRa Library for Arduino

Install the library using the Arduino Library Manager.
License

This project is licensed under the MIT License.
