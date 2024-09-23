# ESP32 ICAR-Receiver

This project is an ESP32-based receiver that monitors sensor data, connects to ThingSpeak, and manages power efficiently using deep sleep mode. It includes features for data fetching, display management, and battery protection.

## Code Structure

- **`pins.h`**: Contains pin mappings for the ESP32.
- **`comms.h`**: Manages Wi-Fi connection and data uploads.
- **`OTA.h`**: Handles OTA firmware updates.
- **`display.h`**: Controls the LCD display.
- **`sensors.h`**: Initializes and reads sensor data.

## Features

- Connects to Wi-Fi and retrieves data from two ThingSpeak channels.
- Displays battery status and data on the LCD.
- Implements deep sleep mode to conserve battery.
- Synchronizes time using NTP (Network Time Protocol).

## Setup Instructions

1. Clone the repository to your local machine.
2. Open the project in the Arduino IDE.
3. Install required libraries (e.g., HTTPClient, ArduinoJson).
4. Select the correct board (DOIT ESP32 DEVKIT V1) and upload the code.
5. Monitor the serial output for status updates.

## Usage

- Upon startup, the device initializes sensors, connects to Wi-Fi, and synchronizes the time.
- Sensor data is fetched from ThingSpeak every few seconds and displayed on the LCD.
- If the battery voltage is below a certain threshold, a warning is displayed, and the device enters deep sleep mode for power conservation.

## Power Management

The device utilizes the ESP32's deep sleep functionality to save battery life. It wakes up periodically to check sensor data and perform necessary updates.

## License

This project is licensed under the MIT License.

## Acknowledgements

- Adafruit for the sensor libraries.
- The ESP32 community for support.

