# ESP32 Stepper and DC Motor Control with Web Server

This project demonstrates how to control two stepper motors and one DC motor using an ESP32 web server. Users can control the direction and number of steps for each stepper motor through a web interface. The DC motor is controlled via PWM and can move forwards, backwards, and at varying speeds.

## Features
- Control **two stepper motors** via a web interface.
- Control a **DC motor** with adjustable speed and direction.
- Built-in web server for motor control using HTTP GET and POST requests.
- Real-time motor status displayed in the serial monitor.

## Components Required
- ESP32
- 2x Stepper motors (with ULN2003 motor driver)
- DC motor
- Motor driver (BTS7960 or similar)
- Jumper wires
- Breadboard
- Power supply (depending on motor specifications)

## Hardware Setup
### Stepper Motors:
- Motor 1 connected to GPIO 19, 18, 5, and 17.
- Motor 2 connected to GPIO 16, 4, 2, and 15.

### DC Motor:
- Motor control pins connected to GPIO 27 and 26.
- PWM signal for motor speed control is set to GPIO 14.

## Software Setup

### 1. Install ESP32 Libraries
- Ensure you have the ESP32 board package installed in Arduino IDE.
- You’ll also need the following libraries:
  - **WiFi.h**: for connecting the ESP32 to the WiFi network.
  - **AsyncTCP.h**: for asynchronous TCP communication.
  - **ESPAsyncWebServer.h**: to serve the web page and handle HTTP requests.
  - **Stepper.h**: for controlling the stepper motors.

### 2. Upload Code
- Modify the following lines in the code to match your WiFi credentials:
  ```cpp
  const char* ssid = "YOUR_SSID";
  const char* password = "YOUR_PASSWORD";
## Upload the code to the ESP32 via Arduino IDE

### Web Interface

Access the web interface by typing the IP address of the ESP32 (shown in the serial monitor) into your browser.

The web page allows you to:
- Set the direction (`Clockwise` or `Counterclockwise`) for both motors.
- Set the number of steps for each motor.
- Click "GO!" to execute the command.

### Usage

1. **Connect the ESP32** to your local WiFi network.
2. **Access the web server** using the ESP32's IP address (printed in the serial monitor).
3. **Control Stepper Motors**: Use the web page to set direction and steps for each motor.
4. **Control DC Motor**: The DC motor will automatically perform forward, backward, and speed increase operations based on the predefined loop code.

## Code Overview

### Main Components

- **WiFi Setup**: Initializes the ESP32 in station mode and connects to the local WiFi network.
- **Web Server**: Handles HTTP GET and POST requests to control the stepper motors.
- **DC Motor Control**: Controls the DC motor's speed and direction using PWM.
- **Stepper Motor Control**: Moves the stepper motors based on the web form inputs.

### HTML Web Interface

A minimal HTML form allows you to select the direction and input the number of steps for each motor. The form is submitted using the `POST` method.

```html
<form action="/" method="POST">
  <input type="radio" name="direction1" value="CW" checked>Clockwise
  <input type="radio" name="direction1" value="CCW">Counterclockwise
  <input type="number" name="steps1">
  <input type="submit" value="GO!">
</form>
PWM Control for DC Motor

The PWM signal is generated using the ledcWrite() function to gradually increase the speed of the DC motor.
