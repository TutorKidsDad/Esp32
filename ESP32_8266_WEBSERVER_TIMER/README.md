# ESP Web Server: Output Control with Slider and Button

This project sets up a web server on an ESP32 or ESP8266 to control an output device (e.g., an LED) using a web interface. Users can toggle the output on and off via a button and adjust the timing or other parameters using a slider.

## Features

- Control an output device (e.g., LED) via a web page.
- Slider for controlling a value (like a timer).
- Wi-Fi connection handling with feedback on Serial Monitor.
- Asynchronous web server for fast, non-blocking responses.
- Compatibility with both ESP32 and ESP8266.

## Prerequisites

### Hardware Requirements

- ESP32 or ESP8266 microcontroller
- LED or any other output device connected to a GPIO pin
- Resistor (if using an LED)
- Wi-Fi network for connecting the ESP device

### Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/Main/Software) with the following libraries installed:
  - **ESPAsyncWebServer**
  - **AsyncTCP** (for ESP32) or **ESPAsyncTCP** (for ESP8266)
  - **WiFi.h** (for ESP32) or **ESP8266WiFi.h** (for ESP8266)

## Installation

1. **Install the Required Libraries**:
   Open Arduino IDE, navigate to **Sketch** > **Include Library** > **Manage Libraries**, and search for the following libraries to install them:
   - ESPAsyncWebServer
   - AsyncTCP (for ESP32)
   - ESPAsyncTCP (for ESP8266)

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/your-repository.git
   cd your-repository

## Hardware Connections

    Connect an LED or other output device to GPIO 2 (default pin in the code).
    If using an LED, connect a suitable resistor in series with it to limit current.

## Upload the Code

    Connect your ESP32 or ESP8266 to your computer.
    Select the appropriate board and port from Tools > Board and Tools > Port.
    Upload the code to the microcontroller.

## Usage
### Monitor Serial Output

After uploading, open the Serial Monitor (Tools > Serial Monitor) to view the Wi-Fi connection status. Once connected, the ESP32/ESP8266 will print its local IP address.
Access the Web Interface

Open a web browser and enter the IP address shown in the Serial Monitor. You will see a web page that allows you to control the output device and adjust the slider value.
##Code Explanation
###Wi-Fi Connection

The device connects to the specified Wi-Fi network and prints its local IP address once connected.
###Asynchronous Web Server

The asynchronous web server allows users to interact with the output device without blocking the main loop, resulting in smoother and faster response times.

