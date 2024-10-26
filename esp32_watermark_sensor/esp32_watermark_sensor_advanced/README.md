Watermark Soil Moisture Sensor project with ESP32:
---
# Watermark Soil Moisture Sensor with ESP32

This project utilizes an ESP32 microcontroller to measure soil moisture levels using a Watermark soil moisture sensor. The readings are displayed on an I2C LCD and made accessible via a simple web interface. The ESP32 acts as a Wi-Fi access point, allowing users to connect and view moisture data on their mobile devices.

## Features

- Measures soil moisture and resistance values.
- Displays readings on a 16x2 I2C LCD.
- Provides a web interface to view moisture readings.
- Supports access via a hostname (e.g., `http://soilmoisture.local`).

## Components Required

- ESP32 Development Board
- Watermark Soil Moisture Sensor
- 16x2 I2C LCD
- Jumper wires
- Breadboard (optional)

## Wiring Diagram

- Connect the Watermark soil moisture sensor to the ESP32 according to your preferred pin configuration.
- Connect the I2C LCD (usually to pins 21 and 22 on the ESP32).

## Libraries Required

Ensure you have the following libraries installed in your Arduino IDE:

- `Wire` (for I2C communication)
- `LiquidCrystal_I2C` (for LCD display)
- `WiFi` (for Wi-Fi capabilities)
- `WebServer` (for serving web pages)
- `ESPmDNS` (for mDNS support)

## Code Overview

### Key Functions

- **setup()**: Initializes the serial communication, LCD, Wi-Fi access point, and mDNS.
- **loop()**: Handles client requests, reads sensor values, and updates the LCD display.
- **handleRoot()**: Serves the HTML page displaying the moisture readings.
- **switchCurrentPath()**: Switches between different current paths for the sensor.
- **sortReadings()**: Sorts the moisture readings to obtain the median value.

### mDNS Support

The ESP32 is configured to respond to `soilmoisture.local`, allowing easy access to the web interface without needing to remember the IP address.

## Usage

1. Upload the code to your ESP32 using the Arduino IDE.
2. Open the Serial Monitor to find the IP address of the access point, usually `192.168.4.1`.
3. Connect your mobile device to the Wi-Fi network named `SoilMoistureAP` using the password you set.
4. Open a web browser and navigate to `http://soilmoisture.local` to view the soil moisture readings.

## Calibration

You may need to calibrate the moisture sensor to ensure accurate readings based on your specific soil conditions. Adjust the resistance values in the code as necessary.

## Troubleshooting

- **Cannot connect to the web interface**: Ensure your mobile device is connected to the `SoilMoistureAP` network.
- **No readings on the LCD or web page**: Check the wiring of the sensor and LCD. Ensure the sensor is correctly powered and connected.

## License

This project is open-source. Feel free to modify and use it for personal or educational purposes.

---

