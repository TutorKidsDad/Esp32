# ESP32 Servo Controller via Web Interface

This project allows you to control a servo motor using an ESP32 and a web interface. The ESP32 serves as a web server, which hosts a simple web page with a slider to control the position of the servo motor.

## Features
- **Web Interface**: Adjust the servo motor's position via a slider on a web page.
- **Wi-Fi Connectivity**: The ESP32 connects to a Wi-Fi network and hosts a web server.
- **Real-Time Control**: The slider value is sent to the ESP32 using AJAX for smooth control of the servo.

## Hardware Required
- **ESP32 Development Board**
- **Servo Motor**
- **Jumper Wires**
- **External Power Supply** (if required by the servo motor)

### Wiring Setup
| ESP32 Pin | Servo Pin       |
|-----------|-----------------|
| GPIO 13   | Signal (Yellow)  |
| 5V        | VCC (Red)        |
| GND       | GND (Black)      |

Ensure that your servo motor is connected correctly to avoid damaging the ESP32 or the motor.

## Software Requirements
- **Arduino IDE**: [Download here](https://www.arduino.cc/en/software)
- **ESP32 Core for Arduino**: Install following the [ESP32 Setup Guide](https://github.com/espressif/arduino-esp32).
- **ESP32Servo Library**: Download and install the library from the Arduino IDE Library Manager or from the [GitHub repository](https://github.com/RoboticsBrno/ESP32-Arduino-Servo-Library).

## How It Works
1. **Wi-Fi Connection**: The ESP32 connects to your Wi-Fi network using the provided SSID and password.
2. **Web Server**: The ESP32 sets up a web server that serves an HTML page. This page features a slider that controls the servo motor.
3. **Servo Control**: When the slider is adjusted, the value is sent to the ESP32 via AJAX, which then moves the servo motor to the corresponding position.

## Steps to Use
1. **Hardware Setup**: Connect the servo motor to the ESP32 according to the wiring diagram above.
2. **Software Setup**:
   - Install the necessary libraries in the Arduino IDE.
   - Enter your Wi-Fi SSID and password in the code.
   - Upload the code to your ESP32.
3. **Access the Web Interface**: 
   - Open the serial monitor in the Arduino IDE to find the ESP32's IP address.
   - Enter the IP address in your browser to access the web interface.
4. **Control the Servo**: Use the slider on the web page to adjust the servo motor's position in real time.

## Project Overview
This project demonstrates the basic implementation of a web-based control system using ESP32 and servo motors. You can easily expand it by adding more controls or sensors to create an interactive IoT project.

### Notes
- Ensure your servo motor is powered correctly. Some servos may require an external power source, especially under load.
- The web interface can be accessed from any device connected to the same Wi-Fi network as the ESP32.

## License
This project is licensed under the MIT License. You are free to use and modify it according to your needs.

