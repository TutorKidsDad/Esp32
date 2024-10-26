
# ESP32 Dual Relay Web Server with WiFiManager

This project uses an ESP32 (or ESP8266) to control two relays through a web-based interface. It also incorporates the **WiFiManager** library for easy Wi-Fi configuration and features an mDNS responder for simplified access.

## Project Overview

The project demonstrates:

- Hosting a web server on the ESP32 (or ESP8266) to control two relays via HTTP requests.
- Serving a web page that allows toggling of two relays using buttons styled as switches.
- Real-time updates of relay states via HTTP requests.
- Configuration of Wi-Fi credentials using **WiFiManager** with an auto-configure captive portal.
- mDNS setup to allow users to access the ESP32 using a simple hostname (`sensor1.local`).

## Hardware Requirements

- ESP32 (or ESP8266) Development Board
- Two Relays
- Two Switches or Push Buttons
- Breadboard and connecting wires

### GPIO Pin Assignments

- **Relay 1:** GPIO 15
- **Switch 1:** GPIO 12
- **Relay 2:** GPIO 2
- **Switch 2:** GPIO 14

## Software Requirements

- **Arduino IDE** (or PlatformIO) with the following installed:
  - **ESP32/ESP8266 Board Package**
  - **WiFiManager** library
  - **ESPAsyncWebServer** library
  - **AsyncTCP** (for ESP32) or **ESPAsyncTCP** (for ESP8266)
  - **LittleFS** (if needed)
  - **Arduino_JSON** (if needed)

## Installation and Setup

1. **Clone or Download the Project**  
   Clone this repository or download the ZIP file and extract it into your Arduino sketch folder.

2. **Install Necessary Libraries**  
   Open the Arduino IDE and install the required libraries using the **Library Manager** (`Sketch > Include Library > Manage Libraries...`):
   - `WiFiManager`
   - `ESPAsyncWebServer`
   - `AsyncTCP` (ESP32) or `ESPAsyncTCP` (ESP8266)

3. **Upload the Code to ESP32/ESP8266**  
   Connect your ESP32/ESP8266 board to your computer, select the appropriate board and port from the **Tools** menu, and upload the sketch.

4. **Configure Wi-Fi Credentials**  
   - On the first boot, the ESP32/ESP8266 will create a Wi-Fi access point named `AutoConnectAP`.
   - Connect to this Wi-Fi network using your computer or mobile device. It should automatically redirect you to a captive portal page.
   - In the captive portal, select your Wi-Fi network and enter the password.

5. **Access the Web Interface**  
   After successful connection, the ESP32 will display its IP address in the serial monitor. Use this IP address or `sensor1.local` in a browser to access the web interface.

## How It Works

- The web server serves an HTML page with toggle switches to control two relays.
- When a switch is toggled, it sends an HTTP request to update the relay state on the ESP32.
- The ESP32 receives these requests and updates the relay states accordingly.
- The webpage periodically polls the ESP32 for the current relay state and updates the web interface in real-time.
- **WiFiManager** is used to configure Wi-Fi credentials and manage connections.

## File Structure

```
ESP32_DualRelay_WebServer/
│
├── ESP32_DualRelay_WebServer.ino // Main project code
├── README.md                     // Project documentation (this file)
└── data/                         // LittleFS folder for web files (if needed)
```

## Dependencies

- **WiFiManager** - Handles Wi-Fi connection and configuration.
- **ESPAsyncWebServer** - Allows asynchronous HTTP server and WebSocket communication.
- **AsyncTCP** or **ESPAsyncTCP** - Provides TCP communication for AsyncWebServer.
- **LittleFS** (if using to serve external files).

## License

This project and code are released under the MIT license.

## Author

**Updated and Modified by Swapnil Karambelkar

---


