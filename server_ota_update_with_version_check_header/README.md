# ESP32 OTA Update with DHT11 Sensor Integration

This project demonstrates how to implement Over-the-Air (OTA) firmware updates and integrate DHT11 sensor readings (temperature and humidity) on an ESP32 board.

## Features

- **OTA Update**: The ESP32 checks for a newer firmware version hosted on a server and performs an OTA update if available.
- **Deep Sleep**: The ESP32 goes into deep sleep mode for power saving when not connected to WiFi or after completing the OTA check.
- **DHT11 Sensor**: Reads temperature and humidity data from a DHT11 sensor and outputs the results via the serial monitor.
- **Analog Input**: Reads and prints the analog value from pin 34.

## Project Structure

- **Main Sketch (`.ino`)**: Handles the main logic, including WiFi connection, OTA updates, and sensor data processing.
- **`ota_update.h`**: Provides functions for WiFi connection, OTA version check, and deep sleep management.
- **`dht_sensor.h`**: Contains functions to initialize and read data from the DHT11 sensor.

## Hardware Requirements

- ESP32 board
- DHT11 sensor
- Analog sensor (connected to pin 34)
- WiFi connection for OTA updates

## Wiring

- **DHT11 Sensor**:
  - Data Pin: GPIO 14 (modifiable in `dht_sensor.h`)
  - Power and Ground to 3.3V and GND on the ESP32

- **Analog Sensor**: Connected to GPIO 34

## Dependencies

Install the following libraries in the Arduino IDE:

1. [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)
2. [HTTPClient](https://github.com/espressif/arduino-esp32/tree/master/libraries/HTTPClient)
3. [Update](https://github.com/espressif/arduino-esp32/tree/master/libraries/Update)
4. [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)

## Setup Instructions

1. Clone or download this repository.
2. Open the `.ino` file in the Arduino IDE.
3. Replace the `ssid` and `password` variables in the main sketch with your WiFi credentials.
4. Flash the ESP32 board with the code.
5. The ESP32 will connect to WiFi and check for any firmware updates on the server.
6. Once connected, the board will continuously read and display the analog value from pin 34, temperature, and humidity data from the DHT11 sensor.

## OTA Update Process

1. The ESP32 checks for the latest firmware version from the `version_url` defined in `ota_update.h`.
2. If a newer version is available, it will download the firmware from the `firmware_url`.
3. The board then reboots with the updated firmware.

## Deep Sleep Mode

If the device fails to connect to WiFi or after the OTA update check, the ESP32 enters deep sleep mode for 5 minutes (configurable in `ota_update.h`).

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

