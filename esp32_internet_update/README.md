ESP32 OTA Update Project using WiFiManager:
---

# ESP32 OTA Firmware Update Project using WiFiManager

## Overview

This project demonstrates how to perform Over-the-Air (OTA) firmware updates on an ESP32 microcontroller using Wi-Fi and a configurable Wi-Fi setup with the **WiFiManager** library. The project checks for new firmware updates from a specified URL periodically and automatically updates the firmware if a newer version is found. It uses WiFiManager to simplify the process of connecting to Wi-Fi by creating a captive portal.

## Features

- **WiFiManager Integration**: Automatically sets up a Wi-Fi connection using a simple web-based captive portal.
- **Firmware Version Check**: Compares the current firmware version with the new one to determine if an update is needed.
- **HTTP-based OTA Update**: Downloads and installs the new firmware from a specified URL.
- **Automatic Reboot**: Reboots the ESP32 after a successful firmware update.
- **Configurable Interval**: Checks for new updates every 60 seconds by default (adjustable).

## Hardware Requirements

- ESP32 development board
- Micro USB cable

## Software Requirements

- Arduino IDE with the ESP32 board support installed
- Required Libraries:
  - **WiFi**
  - **HTTPClient**
  - **Update**
  - **WiFiManager** (Install from the Arduino Library Manager)

## Project Setup

### 1. Install Required Libraries

Open Arduino IDE and navigate to **Sketch** > **Include Library** > **Manage Libraries...**. Search for and install the following:

- **WiFiManager** by tzapu
- **HTTPClient** (included with the ESP32 core)
- **Update** (included with the ESP32 core)

### 2. Configure the Code

In the `ESP32_OTA_Update_WiFiManager.ino` file, update the following values:

```cpp
#define CURRENT_FIRMWARE_VERSION "1.0.0" // Define your current firmware version
const char* firmwareUrl = "http://your_website.com/firmware.bin";
```

- Replace `"1.0.0"` with the version number of your current firmware.
- Replace `http://your_website.com/firmware.bin` with the URL where your firmware binary (`.bin` file) is hosted.
- Ensure that the server hosting the firmware binary sends the `X-Firmware-Version` HTTP header with the version number.

### 3. Upload the Code

1. Connect your ESP32 board to your computer via a USB cable.
2. Open the Arduino IDE, select the correct board and port under **Tools**.
3. Upload the code to your ESP32.

### 4. Setting Up Wi-Fi Using WiFiManager

When the ESP32 is powered on for the first time, it will automatically create a Wi-Fi access point named **`OTA-Setup-AP`**. Follow these steps:

1. Connect to the **`OTA-Setup-AP`** network using your smartphone or computer.
2. A captive portal will open automatically (if not, go to `http://192.168.4.1/` in your web browser).
3. Select your home Wi-Fi network and enter the password.
4. Once configured, the ESP32 will automatically connect to your Wi-Fi network in the future.

### 5. How the Code Works

- The ESP32 will attempt to connect to the saved Wi-Fi network using WiFiManager.
- Once connected, the code will:
  - Make an HTTP GET request to the specified `firmwareUrl`.
  - Check if a new firmware version is available using the HTTP header `X-Firmware-Version`.
  - Compare the version with the `CURRENT_FIRMWARE_VERSION` defined in the code.
  - If a new version is found, download the firmware binary and update the ESP32’s flash memory.
  - Reboot the ESP32 after a successful update.
- If the ESP32 is already up-to-date, it will print a message to the Serial Monitor.

## Code Explanation

### Main Components

1. **WiFiManager Integration**:
   - The code uses WiFiManager to create a simple access point and web portal for Wi-Fi configuration.
   
2. **Firmware Update Process**:
   - The ESP32 connects to the specified firmware URL and checks if the binary is available.
   - Compares the firmware version header from the server with the current firmware version defined in the code.
   - If a new version is available, the ESP32 downloads and installs it using the `Update` library.


## Usage Instructions

1. **Connect to the Wi-Fi AP created by the ESP32**.
2. **Configure Wi-Fi through the captive portal**.
3. **Upload a new firmware version to your server** when needed.
4. **The ESP32 will automatically check and update itself**.

## Troubleshooting

- **Wi-Fi Connection Issues**: If the ESP32 fails to connect to Wi-Fi, try resetting the device and reconfiguring the Wi-Fi credentials.
- **Firmware Update Failure**: Ensure that the firmware URL is accessible and the `.bin` file is not empty. Double-check the HTTP headers if you are using version control.

## License

This project is open-source and licensed under the MIT License. Feel free to modify and distribute as needed.

