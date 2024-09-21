# ESP32 OTA Update and Deep Sleep Example

This repository contains a simple example for performing Over-the-Air (OTA) firmware updates on an ESP32 device and putting it into deep sleep to conserve power. The code connects to a Wi-Fi network, checks a server for a new firmware version, downloads the update if available, and applies it. If no update is needed, the device enters deep sleep for 5 minutes.

## Features

- **Wi-Fi Connectivity**: Connects to a specified Wi-Fi network to check for firmware updates.
- **OTA Firmware Update**: Automatically checks a server for a new firmware version. If a new version is available, it downloads and installs the update.
- **Deep Sleep Mode**: If no update is needed, or if Wi-Fi fails to connect, the device enters deep sleep for a specified period (5 minutes in this example).
- **HTTP Client**: Used to request and fetch the latest firmware version and the update binary file.

## Components Used

- **ESP32**: The microcontroller board used for the example.
- **Wi-Fi**: To connect to the internet and fetch firmware updates.
- **HTTPClient**: Handles HTTP requests to get the firmware version and binary files.
- **esp_sleep**: Handles deep sleep functionality to save power.

## Hardware Required

- **ESP32 Development Board**
- **Wi-Fi Access Point** with internet connectivity

## How It Works

1. **Connect to Wi-Fi**: The device will try to connect to the Wi-Fi network with the credentials you provide.
2. **Check for Firmware Update**: Once connected, the device checks the server for the latest firmware version.
3. **Download and Apply Update**: If a new version is available, the device downloads and installs the update.
4. **Deep Sleep**: After checking for updates (or if Wi-Fi fails to connect), the device enters deep sleep mode for 5 minutes to save power.

## Configuration

- **Wi-Fi Credentials**: Update the `ssid` and `password` variables in the code with your network details.

    ```cpp
    const char* ssid = "your_SSID";
    const char* password = "your_PASSWORD";
    ```

- **Firmware URLs**: The URLs where the firmware version and binary files are hosted. Update `version_url` and `firmware_url` with your own server URLs.

    ```cpp
    const char* version_url = "http://your-server.com/version.txt";
    const char* firmware_url = "http://your-server.com/firmware.bin";
    ```

- **Deep Sleep Time**: The duration for which the ESP32 will sleep (in seconds). Modify this to change the sleep interval.

    ```cpp
    #define TIME_TO_SLEEP 300  // Sleep for 5 minutes
    ```

## How to Use

1. **Clone this repository**:
    ```bash
    git clone https://github.com/yourusername/esp32-ota-sleep-example.git
    ```
   
2. **Upload the code** to your ESP32 board using the Arduino IDE or PlatformIO.

3. **Monitor via Serial**: Open the Serial Monitor at a baud rate of `115200` to view logs about the Wi-Fi connection, OTA updates, and deep sleep status.

4. **Host the Firmware Files**:
    - Host the `version.txt` file on your server with the latest version number.
    - Host the firmware binary (`.bin`) file on the server.
  
5. **Power your ESP32** and watch it connect to Wi-Fi, check for updates, and enter deep sleep if no updates are required.

## Example Server Setup

To provide OTA updates, you need to host:
- A `version.txt` file with the latest firmware version (e.g., `v1.0.1`).
- A binary file (`.bin`) with the compiled firmware.

Place these files on a publicly accessible server.

