# ESP32 Sensor Data Logger with ThingSpeak

## Project Overview

This project demonstrates how to use an ESP32 microcontroller to read sensor data and post it to a ThingSpeak channel. The ESP32 automatically connects to a Wi-Fi network using **WiFiManager** and allows the user to configure the **API key** via a captive portal. It also provides visual feedback through LEDs to indicate Wi-Fi connection status and data transmission.

## Features

- **Dynamic Wi-Fi Management**: Uses WiFiManager to auto-connect to a saved Wi-Fi network or create a captive portal if no connection is available.
- **Dynamic API Key Input**: Users can input or change the ThingSpeak API key through the WiFiManager portal without needing to re-flash the ESP32.
- **Data Logging to ThingSpeak**: Sends sensor data to ThingSpeak at regular intervals.
- **Visual Status Indicators**: Uses LEDs to indicate successful Wi-Fi connection or connection issues.

## Hardware Requirements

- **ESP32 Development Board**
- **Two LEDs** for status indication
- **Any Sensor** (e.g., temperature, humidity sensor, etc.)
- **Resistors** for the LEDs (if required)

## Software Requirements

- **Arduino IDE** (with ESP32 support)
- **WiFiManager Library**
- **HTTPClient Library**

## Circuit Diagram

- **LED 1** is connected to **GPIO 4** (used to indicate successful Wi-Fi connection).
- **LED 2** is connected to **GPIO 5** (used to indicate Wi-Fi connection attempts).

### Pin Configuration

- **LED 1 Pin**: GPIO 4
- **LED 2 Pin**: GPIO 5

## Getting Started

### Step 1: Install the Required Libraries

- **WiFiManager**: [WiFiManager Library](https://github.com/tzapu/WiFiManager)
- **HTTPClient**: Pre-installed in the Arduino core for ESP32

### Step 2: Upload the Code

1. Open the **`main.cpp`** file in the Arduino IDE.
2. Update the LED pin definitions if you are using different GPIO pins.
3. Flash the code to your ESP32.

### Step 3: Connect to the Captive Portal

1. Power on the ESP32.
2. If no saved Wi-Fi network is available, the ESP32 will create a Wi-Fi network named `ESP32_AP` by default.
3. Connect to this network using a mobile or computer.
4. The captive portal should automatically open. If not, open a web browser and navigate to `192.168.4.1`.
5. In the portal, select your Wi-Fi network and enter its password.
6. Enter your ThingSpeak API key.

### Step 4: Monitor Sensor Data

The ESP32 will read sensor data at regular intervals (default: every 5 seconds) and post it to ThingSpeak. You can monitor your data through your ThingSpeak channel.


## How to Change API Key

1. **Reset WiFi Settings:** Press and hold the reset button on your ESP32 to reset the saved Wi-Fi settings in WiFiManager.
2. **Reconnect to Captive Portal:** Follow the same steps to reconnect and enter a new API key.

## Troubleshooting

- **Wi-Fi Connection Issues:** If the ESP32 fails to connect to Wi-Fi, it will enter captive portal mode. Ensure that you have entered the correct Wi-Fi credentials.
- **ThingSpeak Data Not Posting:** Check the API key in the serial monitor output and confirm it matches the one in your ThingSpeak channel settings.

## License

This project is open-source under the [MIT License](LICENSE).

