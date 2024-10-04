# ESP32 Captive Portal WiFi Manager

This project demonstrates how to create a captive portal using the ESP32 to manage WiFi connections. It allows the user to input the SSID and password via a web interface served by the ESP32 in AP mode, then attempts to connect to the provided WiFi credentials.

## Features

- ESP32 in Access Point (AP) mode to create a captive portal
- Web interface for entering WiFi credentials (SSID and Password)
- Automatically connects to the provided WiFi network
- Stores SSID and Password in ESP32's Preferences for future use
- Automatically retries if incorrect credentials are provided

## Requirements

- ESP32 development board
- [Arduino IDE](https://www.arduino.cc/en/software) with ESP32 core installed
- Required libraries:
  - [DNSServer](https://github.com/espressif/arduino-esp32/tree/master/libraries/DNSServer)
  - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
  - [Preferences](https://github.com/espressif/arduino-esp32/tree/master/libraries/Preferences)

## How It Works

1. On first boot, the ESP32 runs in AP mode and serves a simple web page where the user can enter the SSID and Password of the desired WiFi network.
2. After submitting the form, the ESP32 attempts to connect to the provided network.
3. If successful, the WiFi credentials are saved in the ESP32's non-volatile memory (NVS) using the Preferences library.
4. If the credentials are invalid or WiFi connection fails, the device reverts to AP mode and the user can re-enter the credentials.

## Circuit Diagram

- **ESP32 Connections**:
  - No additional hardware connections required for the captive portal setup.

## Code Overview

The core components of the project:

- **Captive Portal HTML**: The web form for entering the SSID and Password is served from `index_html[]` stored in program memory.
- **CaptiveRequestHandler**: A custom handler that serves the captive portal web page for all incoming requests.
- **WiFi Setup**: Functions to handle switching between AP mode and Station mode for connecting to WiFi.

### Key Functions

- `WiFiSoftAPSetup()`: Sets up the ESP32 in AP mode.
- `WiFiStationSetup()`: Connects the ESP32 to the provided WiFi network using the given SSID and Password.
- `StartCaptivePortal()`: Starts the captive portal server and DNS server.

## Getting Started

### 1. Clone the repository:

```bash
git clone https://github.com/your-repo/esp32-captive-portal.git
### 2. Install the required libraries:

- Download and install the **ESPAsyncWebServer** and **DNSServer** libraries from the Arduino Library Manager or GitHub.

### 3. Flash the code:

- Open the project in Arduino IDE.
- Select the appropriate ESP32 board from **Tools > Board**.
- Connect your ESP32 to the computer and upload the code.

### 4. Test the captive portal:

- After the ESP32 boots, it will create an access point called `esp-captive`.
- Connect to this access point using your phone or laptop.
- A web page will automatically open, asking for the WiFi SSID and Password.

---

### Usage

1. Connect to the `esp-captive` WiFi network.
2. Open the web page that appears automatically or go to `http://192.168.4.1/` in your browser.
3. Enter the WiFi SSID and Password.
4. Wait for the ESP32 to attempt the connection.

---

### Debugging

- Use the Serial Monitor (set baud rate to **115200**) to view the debug messages and check for issues during the WiFi connection process.
- If the connection times out, the captive portal will restart, allowing you to enter new credentials.

