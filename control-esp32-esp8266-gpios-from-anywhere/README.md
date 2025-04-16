
# ESP32 Wi-Fi Controlled GPIOs with WiFiManager and mDNS

This project demonstrates how to control the GPIOs of an ESP32 over a Wi-Fi network using HTTP requests. It includes an easy-to-configure captive portal using **WiFiManager** and enables the use of a friendly domain name using **mDNS**.

## Project Overview

The project allows you to control GPIOs remotely via HTTP requests sent to the ESP32. WiFiManager simplifies the initial setup by providing a captive portal to connect to your preferred Wi-Fi network. Additionally, the project utilizes mDNS to provide a human-readable domain name instead of an IP address, e.g., `http://sensor1.local`.

### Key Features:
- **Captive Portal with WiFiManager**: Allows easy setup and connection to your Wi-Fi network.
- **mDNS Hostname**: Access your ESP32 via `http://sensor1.local`.
- **HTTP-based Control**: Control GPIOs with remote HTTP requests.
- **Deep Sleep on Connection Failure**: Saves power by entering deep sleep if no connection is established.

## Table of Contents

- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Circuit Diagram](#circuit-diagram)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Configuration](#configuration)
- [Accessing the ESP32](#accessing-the-esp32)
- [Troubleshooting](#troubleshooting)
- [License](#license)
- [Acknowledgements](#acknowledgements)

## Hardware Requirements

- **ESP32 Development Board**
- **LEDs, Relays, or any GPIO-controlled device**
- **Micro USB cable** for flashing the ESP32

## Software Requirements

- **Arduino IDE** (1.8.10 or higher) or **PlatformIO**
- **ESP32 Board Package** installed in the Arduino IDE
- **WiFiManager Library** installed in the Arduino IDE (`tzapu/WiFiManager`)
- **Arduino JSON Library** installed in the Arduino IDE (`arduino-libraries/Arduino_JSON`)

## Circuit Diagram

Below is a simple circuit setup where the ESP32 is connected to several LEDs. You can expand this to control other devices like relays or sensors.

### Example Circuit:

- **GPIO 4** - LED 1
- **GPIO 5** - LED 2
- **GPIO 12** - Relay 1

## Getting Started

### Prerequisites

- Install the **Arduino IDE** or **PlatformIO**.
- Install the required ESP32 board support and libraries.

### Installation

1. **Clone this repository** or download it as a ZIP file and extract it.

   ```bash
   git clone https://github.com/yourusername/ESP32-GPIO-Control.git
   cd ESP32-GPIO-Control
   ```

2. **Open the project** in the Arduino IDE or PlatformIO.
3. **Install required libraries**:
   - **WiFiManager** library (`tzapu/WiFiManager`)
   - **Arduino_JSON** library (`arduino-libraries/Arduino_JSON`)

4. **Select your ESP32 board** from the **Tools > Board** menu in the Arduino IDE.
5. **Configure the `serverName`** in the code to point to your HTTP server (if needed).

### Configuration

1. **SSID and Password**: The ESP32 automatically opens a configuration portal if it can't connect to Wi-Fi.
2. **Captive Portal Password**: You can set a password for the configuration portal by modifying the `autoConnect()` method in the code:

   ```cpp
   wm.autoConnect("ESP32-Setup", "YOUR_PORTAL_PASSWORD");
   ```

3. **mDNS Setup**: The default hostname is set as `sensor1`. You can change it by modifying the `MDNS.begin()` line:

   ```cpp
   if (!MDNS.begin("sensor1")) {
     Serial.println("Error setting up mDNS responder!");
   }
   ```

## Accessing the ESP32

1. **Connect to the Wi-Fi network** configured during the captive portal setup.
2. **Use mDNS** to access the ESP32 from your browser: `http://sensor1.local`
3. **Control GPIOs** by sending an HTTP request to the configured server:

   ```plaintext
   GET http://onlinetps.com/test1/esp-outputs-action.php?action=outputs_state&board=1
   ```

## Troubleshooting

### Common Issues and Solutions:

- **WiFiManager Captive Portal not showing up**:
  Ensure that you disconnect your current Wi-Fi or forget the Wi-Fi network on your phone or computer and connect directly to the "ESP32-Setup" network.

- **ESP32 not connecting to Wi-Fi**:
  Double-check the SSID and password you provided during captive portal setup. You can reset the saved credentials by pressing the reset button on the ESP32.

- **mDNS not working**:
  Ensure your device and the ESP32 are on the same network. If it still doesn't work, you can try accessing the ESP32 directly using its IP address (e.g., `http://192.168.1.x`).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Rui Santos - Original project inspiration and details: [Random Nerd Tutorials](https://RandomNerdTutorials.com/control-esp32-esp8266-gpios-from-anywhere/)


