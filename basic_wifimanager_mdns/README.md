ESP32 project with WiFiManager, mDNS, and an HTTP server to control an LED

---

# ESP32 LED Control with WiFiManager, mDNS, and HTTP Server

This project demonstrates how to control an LED connected to an ESP32 using a web interface. It leverages **WiFiManager** to automatically connect to a Wi-Fi network or provide a captive portal for configuration. It also uses **mDNS** to provide an easy-to-remember URL (`http://ledcontrol.local`) to access the ESP32's web server.

## Features
- **WiFiManager Integration**: Automatically connects to a Wi-Fi network or opens a captive portal to set up the connection.
- **mDNS Service**: Allows users to access the ESP32 using an easy-to-remember hostname (`http://ledcontrol.local`).
- **HTTP Server**: A simple web server to control an LED via a toggle button.

## Prerequisites
- **ESP32 Board**
- **Arduino IDE** with ESP32 support installed
- **WiFiManager Library** installed in the Arduino IDE
- **mDNS Library** is included with ESP32 support

## Hardware Requirements
- **ESP32 Board**
- **LED** and **Current Limiting Resistor** (or use an onboard LED if available)
- **Breadboard and Jumper Wires** (if using an external LED)

## Libraries Required
- `WiFiManager.h`
- `ESPmDNS.h`
- `WiFi.h`

## Circuit Diagram
- **LED** connected to a digital pin of your choice (default is GPIO 2) and the ground via a current-limiting resistor.
- Alternatively, you can use the onboard LED on some ESP32 boards connected to GPIO 2.

## Project Setup

### 1. Upload the Code

1. Open the provided code in the **Arduino IDE**.
2. Ensure that the required libraries are installed (check the Library Manager).
3. Configure the **LED pin** if needed:
   ```cpp
   const int ledPin = 2; // Change to your desired LED pin if using an external LED
   ```
4. Upload the code to your ESP32 board.

### 2. Connect to Wi-Fi

When the ESP32 boots:
- It will automatically attempt to connect using previously saved Wi-Fi credentials.
- If no saved credentials are found, or if it fails to connect, it will open a **captive portal** named `LED_Controller_AP`. You can connect to this portal via your mobile phone or computer to configure the Wi-Fi connection.

### 3. Access the ESP32 Web Server

Once connected to the Wi-Fi:
- Access the web server using mDNS:
  ```bash
  http://ledcontrol.local
  ```
  > **Note**: Ensure that your computer or mobile device supports **mDNS** and is on the same Wi-Fi network as the ESP32. On Windows, installing **Bonjour** may be necessary for mDNS to work.

### 4. Controlling the LED

- When you open the web page, you will see the LED status (ON or OFF) and a button to toggle the LED state.
- Clicking the **Toggle LED** button will change the state of the LED.

## Code Explanation

### `setup()` Function:
- **Initialize Serial Communication**: For debugging and monitoring.
- **Set Up LED Pin**: Configured as an output and set to LOW initially.
- **Create an Instance of WiFiManager**: Automatically connects to Wi-Fi or sets up a captive portal for configuration.
- **Connect to Wi-Fi**: If connection fails, the ESP32 restarts.
- **Set Up mDNS**: Allows accessing the ESP32 using the hostname `ledcontrol.local`.
- **Add mDNS HTTP Service**: Adds an HTTP service to the mDNS responder for easier access.

### `loop()` Function:
- **Create an HTTP Server**: Waits for client connections on port 80.
- **Handle Client Requests**: Parses HTTP requests and sends an HTML page with a button to toggle the LED.
- **Toggle LED**: Checks if the client requested to toggle the LED state and updates the LED accordingly.

## Usage

1. **Power on the ESP32** and ensure it's connected to the configured Wi-Fi network.
2. **Access the ESP32** using the following URL:
   ```
   http://ledcontrol.local
   ```
3. **Control the LED**: Click the **Toggle LED** button on the webpage to switch the LED ON or OFF.

## Troubleshooting

1. **Wi-Fi Connection Issues**:
   - Check if the ESP32 opens a captive portal. If not, check the serial monitor for debug messages.
   - Ensure that you are connected to the correct captive portal (`LED_Controller_AP`) and configure the Wi-Fi settings.

2. **mDNS Not Working**:
   - Ensure that your device supports mDNS and is on the same Wi-Fi network as the ESP32.
   - Try using the ESP32’s IP address (found in the serial monitor) instead of the mDNS hostname.

3. **LED Not Responding**:
   - Double-check the LED wiring and pin configuration.
   - Ensure the correct GPIO pin is set in the code.

---

This project provides a great starting point for controlling devices over a network and understanding the basics of Wi-Fi configuration and mDNS on ESP32. Feel free to expand it with additional features!
