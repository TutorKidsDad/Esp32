# DC Motor Control with ESP32

This project demonstrates how to control a DC motor using an ESP32 with PWM speed control, direction control, and non-blocking delays for smoother motor operation. The motor can move forward and backward, gradually increase its speed, and handle error scenarios.

## Features
- **Forward and Backward Control**: The motor can switch between forward and backward movements based on direction commands.
- **Speed Control**: PWM is used to control the speed of the motor, with gradual speed increases.
- **Non-Blocking Delays**: Uses `millis()` to implement non-blocking delays for motor control, allowing for better multitasking.
- **Error Handling**: A flag (`motorError`) is used to handle motor errors and stop the motor if needed.

## Hardware Requirements
- **ESP32**
- **DC Motor**
- **Motor Driver**
- **External Power Supply for the Motor**

## Pin Configuration
- `motor1Pin1` (GPIO 27): Control pin 1 for motor direction
- `motor1Pin2` (GPIO 26): Control pin 2 for motor direction
- `enable1Pin` (GPIO 14): PWM control pin for motor speed

## PWM Configuration
- **Frequency**: 30,000 Hz
- **Resolution**: 8-bit
- **PWM Channel**: 0

## Code Breakdown

### Setup
- Motor control pins are configured as outputs using `pinMode()`.
- PWM is set up using the `ledcAttach()` function to enable precise speed control over the motor.
- Serial communication is initialized for debugging purposes.

### Main Loop
1. **Forward Movement**: The motor moves forward at maximum speed for a set duration (`motorDelay`).
2. **Stop the Motor**: The motor stops for a short delay (`stopDelay`) after each forward or backward movement.
3. **Backward Movement**: The motor moves backward at maximum speed.
4. **Speed Gradual Increase**: The motor speed is gradually increased in small increments, and this is done using the `ledcWrite()` function for smooth acceleration.

### Functions
- `controlMotor(bool forward)`: This function controls the motor direction. When `forward` is `true`, the motor moves forward, otherwise, it moves backward.
- `stopMotor()`: Stops the motor by disabling the motor driver.
- `gradualSpeedUp()`: Gradually increases the motor speed by adjusting the PWM duty cycle in increments.

### Error Handling
- A simple flag (`motorError`) is used to detect any errors in the motor operation. If an error is detected, the motor stops and the program exits the loop to prevent further operation.

## Future Improvements
- Add functionality to handle more complex motor errors.
- Implement feedback from the motor (e.g., using an encoder) for precise control.
- Allow dynamic speed changes based on user input.

## Example Serial Output

