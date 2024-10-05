# ESP32/ESP8266 Web Server with Physical Button Control

This project demonstrates how to create a web server using an ESP32 or ESP8266 to control a valve through a physical button and a web interface. The system allows users to monitor and control the state of a valve using both a physical button and a toggle switch on a webpage.

## Features
- Control a valve (GPIO pin) using a physical button.
- Toggle the valve state through a web interface.
- Monitor the valve's state in real-time from the web interface.
- Debouncing for reliable button inputs.

## Components
- ESP32 or ESP8266 microcontroller.
- Valve or any output component (connected to GPIO 15).
- Push button (connected to GPIO 32).
- Wi-Fi connection for remote access via the web interface.

## Libraries
The following libraries are required:
- `WiFi.h` (ESP32) or `ESP8266WiFi.h` (ESP8266)
- `AsyncTCP.h` (ESP32) or `ESPAsyncTCP.h` (ESP8266)
- `ESPAsyncWebServer.h`

## Pin Connections
- **Button Pin:** GPIO 32 (Input - for physical button)
- **Valve Pin:** GPIO 15 (Output - controls valve)

## Getting Started

### 1. Install Required Libraries
To run this project, ensure you have the following libraries installed via the Arduino IDE:
- **ESPAsyncWebServer**: [GitHub Repository](https://github.com/me-no-dev/ESPAsyncWebServer)
- **AsyncTCP** (for ESP32): [GitHub Repository](https://github.com/me-no-dev/AsyncTCP)
- **ESPAsyncTCP** (for ESP8266): [GitHub Repository](https://github.com/me-no-dev/ESPAsyncTCP)

### 2. Modify Network Credentials
Replace the following variables with your Wi-Fi credentials in the code:

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";


### 3. Upload the Code
After making the necessary changes, upload the code to your ESP32 or ESP8266.

### 4. Access the Web Server
Once the ESP connects to your Wi-Fi, the local IP address will be printed to the Serial Monitor. Use this IP address to access the web server from any device on the same network.

### Web Interface
The web page features a toggle switch to control the valve and monitor its state in real-time. The switch will reflect the current state of the valve, allowing you to control it remotely.

### Button Debouncing
The physical button is debounced in the code to prevent erroneous state changes caused by noise. If the button is pressed, the valve toggles between ON and OFF states, and this is reflected on the web interface.

## Code Explanation

### Web Interface
The web interface is a simple HTML page that includes a toggle switch. It periodically checks the state of the valve using JavaScript to ensure the switch reflects the current state of the output pin.

### Button Control
The physical button is connected to GPIO 32. The code uses debouncing logic to avoid multiple toggles from a single press. When the button is pressed, the valve (GPIO 15) toggles between HIGH (on) and LOW (off).

### Server Endpoints
- **`/`**: Serves the HTML page.
- **`/update?state=<state>`**: Updates the valve state based on the value (`0` for OFF, `1` for ON).
- **`/state`**: Returns the current state of the valve (`0` or `1`).

## How It Works
1. **Physical Button**: The physical button is debounced using a simple timer-based debounce logic. When the button is pressed, the valve state toggles.
2. **Web Interface**: A toggle switch is presented on the web interface. When the switch is flipped, it sends a request to update the state of the valve. The web page automatically refreshes every second to reflect the current valve state.

## Circuit Diagram
- **Button**: Connect one end of the button to GPIO 32 and the other to GND.
- **Valve**: Connect the valve to GPIO 15, and ensure proper power is supplied to it.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements
The web server and button handling code is adapted from tutorials by Rui Santos, available at [Random Nerd Tutorials](https://RandomNerdTutorials.com).

