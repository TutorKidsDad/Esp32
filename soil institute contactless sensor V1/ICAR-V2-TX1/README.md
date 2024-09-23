# ESP32 Sensor and Display Project

## Overview

This project leverages an ESP32 microcontroller to collect sensor data (temperature, humidity, and light) and display it on an I2C LCD. It includes features for Wi-Fi connectivity, OTA firmware updates, and low-power operation using deep sleep mode.

## Features

- **Wi-Fi Connectivity**: Connects to a specified Wi-Fi network.
- **OTA Updates**: Supports over-the-air firmware updates.
- **Sensor Data Acquisition**: Reads from temperature, humidity, and light sensors.
- **LCD Display**: Shows real-time sensor data and device status.
- **Deep Sleep Mode**: Enters sleep mode to conserve power between readings.

## Pin Configuration


#define SCL 22        // I2C clock
#define SDA 21        // I2C data
#define R_LED 4       // Red LED
#define G_LED 18      // Green LED
#define B_LED 19      // Blue LED
#define STRIP_LED 14  // LED strip
#define MOISTURE_IN 34// Moisture sensor
#define V_BATT 32     // Battery voltage
#define BUZZER 12     // Buzzer

## Code Structure

- **`pins.h`**: Defines pin mappings.
- **`comms.h`**: Manages Wi-Fi connection and data uploads.
- **`OTA.h`**: Handles OTA firmware updates.
- **`sensors.h`**: Initializes and reads sensor data.
- **`display.h`**: Controls the LCD display.

## Setup Instructions

1. Clone the repository to your local machine.
2. Open the project in the Arduino IDE.
3. Install required libraries (e.g., Adafruit sensor libraries, LiquidCrystal).
4. Select the correct board (ESP32) and upload the code.
5. Monitor the serial output for status updates.

## Usage

- The device connects to Wi-Fi and displays status on the LCD.
- It reads sensor data and updates the display every few seconds.
- If battery voltage is low, it shows a warning and enters deep sleep.

## License

This project is licensed under the MIT License.

## Acknowledgements

- Adafruit for the sensor libraries.
- The ESP32 community for support.

