# ESP8266/ESP32 DHT Sensor Web Server

This project creates a simple web server using either an ESP8266 or ESP32 microcontroller. It reads temperature and humidity data from a DHT22 sensor and displays it on a web page. The project is compatible with both ESP8266 and ESP32, thanks to conditional compilation.

## Features

- Connects to a Wi-Fi network.
- Reads temperature and humidity using a DHT22 sensor.
- Serves the sensor data over a web server.
- Automatically detects whether the microcontroller is an ESP8266 or ESP32.

## Components Required

- **ESP8266 or ESP32** microcontroller.
- **DHT22** temperature and humidity sensor.
- **Wi-Fi** connection.

## Wiring

### For ESP8266:
- **DHT22 Data Pin** -> **D3** (can be changed in the code).
- **VCC** -> **3.3V**.
- **GND** -> **GND**.

### For ESP32:
- **DHT22 Data Pin** -> **GPIO 15** (can be changed in the code).
- **VCC** -> **3.3V**.
- **GND** -> **GND**.

## Installation

1. Clone this repository or download the files.
2. Open the `.ino` file in the Arduino IDE.
3. Install the following libraries if you haven't already:
    - [ESP8266WiFi](https://github.com/esp8266/Arduino)
    - [WiFi (for ESP32)](https://github.com/espressif/arduino-esp32)
    - [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
    - [Adafruit Unified Sensor Library](https://github.com/adafruit/Adafruit_Sensor)
4. In the `setup()` function, modify the `ssid` and `password` to match your Wi-Fi network.
5. Select your board (ESP8266 or ESP32) in the **Tools > Board** menu.
6. Upload the sketch to your microcontroller.

## Code Overview

### Conditional Compilation
The code uses `#if defined(ESP8266)` and `#elif defined(ESP32)` to differentiate between ESP8266 and ESP32, ensuring that the correct libraries and pin configurations are used.

### Web Server
- The web server runs on port 80.
- When the server receives a request, it reads the temperature and humidity from the DHT22 sensor and displays the values on a simple HTML page.

### Functions
- `handle_OnConnect`: Reads sensor data and serves it as HTML.
- `handle_NotFound`: Handles requests to unknown routes and returns a 404 error.
- `SendHTML`: Generates the HTML response with the current temperature and humidity.

## Usage

1. After uploading the sketch to your ESP8266 or ESP32, open the **Serial Monitor** at a baud rate of 115200.
2. The microcontroller will connect to the specified Wi-Fi network.
3. Once connected, it will print the device's IP address to the Serial Monitor.
4. Open your browser and navigate to the IP address to see the temperature and humidity values.

## Example Output

### Web Page:

## Troubleshooting

- **Wi-Fi Connection Issues**: Ensure that the SSID and password are correctly entered. Check if the ESP8266/ESP32 is in range of the Wi-Fi router.
- **DHT Sensor Readings**: If you're getting `NaN` (Not a Number) readings, check the wiring and make sure the DHT sensor is connected to the correct pin.
- **ESP32 Compatibility**: Ensure that the correct board (ESP32 or ESP8266) is selected in the **Tools > Board** menu of the Arduino IDE.
