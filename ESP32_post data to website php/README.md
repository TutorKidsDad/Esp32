# IMPROVEMENT IN CODE
Use of JSON:

Instead of sending data as x-www-form-urlencoded,I have used JSON format for structured data transmission, which is more flexible and can be easily parsed on the server-side

# ESP32/ESP8266 Cloud Weather Station

This project demonstrates how to send sensor data from an ESP32/ESP8266 to a remote server using HTTP POST requests. The data can be accessed through a web server for monitoring weather conditions, like temperature, humidity, and pressure.

## Project Details

This project collects data from sensors (like BME280) and posts it to a server. The project can be easily extended to add more sensors or modify the data transmission format (such as JSON).

- **Microcontroller:** ESP32 or ESP8266
- **Network Protocol:** HTTP POST (Wi-Fi)
- **Server:** Remote server handling HTTP POST requests
- **Sensor:** BME280 or any other supported sensor

Complete details can be found at the tutorial: [RandomNerdTutorials](https://RandomNerdTutorials.com/cloud-weather-station-esp32-esp8266/)

## Features

- Connects to a Wi-Fi network.
- Sends sensor data (temperature, humidity, pressure) to a remote server using HTTP POST.
- Handles server responses and displays HTTP status codes.
- Reconnects to Wi-Fi if the connection is lost.
- Configurable interval for sending data.

## Hardware Requirements

- ESP32 or ESP8266
- BME280 sensor (or any supported weather sensor)
- USB cable
- Breadboard and jumper wires (optional)

## Software Requirements

- Arduino IDE with ESP32/ESP8266 board support
- Wi-Fi network

## Setup Instructions

### 1. Install ESP32/ESP8266 Board in Arduino IDE

If you haven't already, follow these instructions to add ESP32 or ESP8266 board support to your Arduino IDE:

- [Installing ESP32 in Arduino IDE](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)
- [Installing ESP8266 in Arduino IDE](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

### 2. Required Libraries

Make sure you have the following libraries installed:

- `WiFi.h` for ESP32 or `ESP8266WiFi.h` for ESP8266
- `HTTPClient.h` for making HTTP POST requests
- Optional: `Adafruit BME280` library for sensor readings

To install the libraries, go to **Sketch** > **Include Library** > **Manage Libraries...** in the Arduino IDE.

### 3. Circuit Diagram

| ESP32/ESP8266 | BME280 Sensor |
|---------------|---------------|
| 3V3           | VIN           |
| GND           | GND           |
| SDA (D21/D2)  | SDA           |
| SCL (D22/D1)  | SCL           |

Connect your ESP32/ESP8266 to the BME280 sensor following the wiring diagram above.

### 4. Code Configuration

- Open the Arduino IDE and load the code provided in `main.ino`.
- Replace the placeholders in the code with your Wi-Fi credentials:
  ```cpp
  const char* ssid = "your-SSID";
  const char* password = "your-PASSWORD";

