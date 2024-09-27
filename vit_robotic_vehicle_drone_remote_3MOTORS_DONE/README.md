# BTS7960 Motor Control with ESP32 and Drone Remote

## Overview

This project demonstrates how to control two motors with bidirectional control and an additional motor with speed-only control using an ESP32 and BTS7960B H-Bridge motor drivers. The motors are controlled via a drone remote, with separate throttle and yaw channels for forward/backward and turning movements, respectively.

### Features:
- Control of two bidirectional motors using a joystick for speed and direction.
- Additional motor with speed-only control, controlled via a separate channel on the drone remote.
- Fault detection for motor drivers, and a failsafe to stop all motors when no signal is received.

## Hardware Components:
- **ESP32**: Main microcontroller.
- **BTS7960B H-Bridge Motor Drivers**: Used to control the motors.
  - Motor 1 and Motor 2 (with bidirectional control).
  - Motor 3 (speed control only).
- **Drone Remote**: Sends PWM signals for throttle (speed) and yaw (direction).

## Pin Configuration:

### Motor 1 (Bidirectional)
| Pin Name          | ESP32 Pin |
|-------------------|-----------|
| RPWM (Forward)    | 26        |
| LPWM (Backward)   | 25        |
| R_EN (Enable)     | 33        |
| L_EN (Enable)     | 32        |
| R_IS (Current Sense) | 35     |
| L_IS (Current Sense) | 34     |

### Motor 2 (Bidirectional)
| Pin Name          | ESP32 Pin |
|-------------------|-----------|
| RPWM (Forward)    | 19        |
| LPWM (Backward)   | 21        |
| R_EN (Enable)     | 22        |
| L_EN (Enable)     | 23        |
| R_IS (Current Sense) | 36     |
| L_IS (Current Sense) | 39     |

### Motor 3 (Speed-Only Control)
| Pin Name          | ESP32 Pin |
|-------------------|-----------|
| PWM (Speed Control) | 14       |
| LPWM (Speed Control) | 12      |
| R_EN (Enable)     | 16        |
| L_EN (Enable)     | 17        |

### Remote Control Input (Drone Remote)
| Pin Name          | ESP32 Pin |
|-------------------|-----------|
| Throttle          | 18        |
| Yaw               | 4         |
| Speed Control (Motor 3) | 27 |

## Functions and Operation

### `setup()`
- Initializes all motor drivers and sets pin modes.
- Prepares the ESP32 to receive input from the drone remote for motor control.

### `loop()`
- Continuously reads the PWM signals from the remote's throttle, yaw, and speed control channels.
- Based on the values of throttle and yaw, controls the direction and speed of Motor 1 and Motor 2.
- Controls the speed of Motor 3 using the additional speed control channel.
- Implements a failsafe to stop all motors if no signal is received from the remote.

### Motor Control Logic
- **Throttle**: Controls the forward and backward speed of Motor 1 and Motor 2. The signal is mapped to a speed range of 0 to 255.
- **Yaw**: Controls the turning of Motor 1 and Motor 2 by rotating the motors in opposite directions to achieve a turn.
- **Additional Motor**: Speed of Motor 3 is controlled separately using a PWM signal from the speed control channel.

### Fault Detection
- The program monitors the current sense (IS) pins of each motor driver to detect faults. If a fault is detected, the respective motor is disabled temporarily and re-enabled after a short delay.

### Failsafe
- If no signal is received from the drone remote (either throttle or yaw signal is 0), all motors are stopped, and a message is printed to the serial monitor.

## Example Output (Serial Monitor)

Throttle: 1600 | Yaw: 1500 | Additional Motor Speed: 100 Forward | Speed: 120 Turning Left | Turn Speed: 80 No signal from transmitter, stopping all motors.

## How to Use

1. Connect the motors, ESP32, and BTS7960B motor drivers as per the pin configuration above.
2. Upload the code to the ESP32.
3. Power the setup and use the drone remote to control the motors:
    - Throttle for forward and backward movement.
    - Yaw for turning left or right.
    - Speed control for the additional motor.
4. Monitor the serial output for debugging information, such as throttle, yaw values, and fault detection status.

## Libraries Used
- `BTS7960.h`: Library for controlling the BTS7960B motor driver.

## License
This project is open-source under the MIT License. Feel free to use and modify it as needed.


