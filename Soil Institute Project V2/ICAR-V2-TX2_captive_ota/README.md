# ESP32 Sensor Monitoring Project

This project is designed for an ESP32 microcontroller to monitor sensor data and manage power efficiently using deep sleep mode. It includes OTA (Over-the-Air) updates, Wi-Fi connectivity, and an LCD display for user interaction.

## Code Structure

- **`pins.h`**: Contains pin mappings for the ESP32.
- **`comms.h`**: Manages Wi-Fi connection and data uploads.
- **`OTA.h`**: Handles OTA firmware updates.
- **`sensors.h`**: Initializes and reads sensor data.
- **`display.h`**: Controls the LCD display.

## Features

- Connects to Wi-Fi and uploads sensor data.
- Displays battery status on the LCD.
- Enters deep sleep mode to conserve power when the battery voltage is low.
- LED indicators for visual feedback during operation.

## Setup Instructions

1. Clone the repository to your local machine.
2. Open the project in the Arduino IDE.
3. Install required libraries (e.g., Adafruit sensor libraries, LiquidCrystal).
4. Select the correct board (ESP32) and upload the code.
5. Monitor the serial output for status updates.

## Usage

- Upon startup, the device initializes sensors, connects to Wi-Fi, and prepares for data collection.
- Sensor data is read and displayed on the LCD every few seconds.
- If the battery voltage drops below 3.5V, a warning is shown, and the device enters deep sleep for 3 minutes to save power.

## Power Management

The device utilizes the ESP32's deep sleep functionality to conserve battery life. It wakes up every 3 minutes to check sensor data and upload it if necessary.

## License

This project is licensed under the MIT License.

## Acknowledgements

- Adafruit for the sensor libraries.
- The ESP32 community for support.

