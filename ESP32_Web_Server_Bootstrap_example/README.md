
# ESP32 Web Server Project

This project demonstrates how to create a web server using an ESP32 microcontroller to control two LEDs and monitor temperature using an LM35 temperature sensor. The web interface allows users to toggle the LEDs and view real-time temperature readings on the web page. The interface is built using HTML, CSS, and Bootstrap for a responsive design.

## Features
- **Control two LEDs (GPIO 26 and GPIO 27)**: The web server provides buttons to turn each LED on or off.
- **Temperature Monitoring (LM35 on GPIO 33)**: The temperature from the LM35 sensor is read, averaged, and displayed on the web interface.
- **Responsive Web Design**: Bootstrap is used to provide a modern and responsive interface that works across various devices.
- **Debouncing for LED Control**: Implements debouncing to avoid accidental multiple triggers when controlling LEDs.
- **Over-The-Air (OTA) Updates**: Supports OTA updates to allow firmware to be updated wirelessly.
- **Wi-Fi Access Point Mode**: The ESP32 creates a Wi-Fi Access Point, and users can connect to the device to control it via a web browser.

## Circuit Diagram
The hardware connections for this project are as follows:

- **LED 1 (GPIO 26)**: Connected to GPIO 26.
- **LED 2 (GPIO 27)**: Connected to GPIO 27.
- **LM35 Temperature Sensor (GPIO 33)**: Connected to GPIO 33 for temperature readings.

## Requirements

### Hardware:
- ESP32 Development Board
- 2 LEDs
- LM35 Temperature Sensor
- Resistors (appropriate values for LEDs)
- Jumper wires
- Breadboard

### Software:
- [Arduino IDE](https://www.arduino.cc/en/software) with ESP32 board support
- Required Arduino Libraries:
  - `WiFi.h` (for Wi-Fi functions)
  - `ArduinoOTA.h` (for OTA updates)

## Installation

1. **Install Arduino IDE and ESP32 Board Support:**
   - Download and install the Arduino IDE from the official website.
   - Follow instructions [here](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/) to install the ESP32 board package in Arduino IDE.

2. **Install Required Libraries:**
   - The required libraries (`WiFi.h` and `ArduinoOTA.h`) are already included with the ESP32 package.

3. **Hardware Setup:**
   - Connect the components (LEDs, resistors, LM35) to the ESP32 as described in the circuit diagram above.
   
4. **Upload Code to ESP32:**
   - Open the modified code in Arduino IDE.
   - Set your correct Wi-Fi credentials in the code:
     ```cpp
     const char* ssid = "your-ssid";
     const char* password = "your-password";
     ```
   - Select your ESP32 board under **Tools > Board > ESP32 Dev Module**.
   - Select the correct port and upload the code.

5. **Access the Web Interface:**
   - After uploading, the ESP32 will create a Wi-Fi Access Point.
   - Connect to this Wi-Fi network (SSID: `testing`, Password: `password`).
   - Open a web browser and go to the ESP32’s IP address (printed on the Serial Monitor).

## How to Use

1. **Controlling LEDs:**
   - After connecting to the ESP32's web interface, you will see buttons to control the two LEDs.
   - Click the **ON** button to turn on the LED and **OFF** to turn it off. The current state of each LED is displayed next to the button.

2. **Monitoring Temperature:**
   - The current temperature from the LM35 sensor is displayed in real-time on the web page.
   - The background color of the temperature reading changes based on the current temperature:
     - Green for cool temperatures (below 21°C).
     - Yellow for warm temperatures (21°C - 24°C).
     - Red for hot temperatures (above 24°C).

3. **OTA Firmware Update:**
   - You can update the firmware wirelessly using Arduino OTA.
   - To trigger an OTA update, go to **Sketch > Upload Using OTA** in the Arduino IDE.

## License
This project is licensed under the GNU General Public License v3.0. You can use and modify it freely under the terms of this license.


