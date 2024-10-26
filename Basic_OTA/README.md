
# ESP32 WiFi Manager & OTA Project

This project demonstrates how to use **WiFiManager**, **mDNS**, and **Over-the-Air (OTA) Updates** on an ESP32 microcontroller. The provided code configures the ESP32 to automatically connect to a Wi-Fi network, set up mDNS for easy access using a hostname, and enables OTA updates for remote firmware uploads.

## Features
- **WiFiManager Integration**: Automatically connects to Wi-Fi using saved credentials or opens a captive portal for setup.
- **mDNS Service**: Allows you to access your ESP32 with a hostname (e.g., `http://sensor1.local`).
- **Over-the-Air (OTA) Updates**: Update your ESP32 firmware wirelessly without needing a physical connection.
- **Monitoring**: Free heap memory monitoring in the loop.
- **Deep Sleep Logic** *(Optional)*: Implement deep sleep functionality after a specified period.

## Prerequisites
- **ESP32 Board**: Ensure you have an ESP32 microcontroller.
- **Arduino IDE**: Installed with the **ESP32 Board** package.
- **WiFiManager Library**: Install it from the Library Manager in Arduino IDE.
- **OTA Capable Network**: You should be connected to the same network as the ESP32 for OTA updates.

## Hardware Requirements
- **ESP32 Board**
- **Micro USB Cable** (for initial programming and power supply)

## Libraries Required
- `WiFi.h`
- `ESPmDNS.h`
- `WiFiUdp.h`
- `ArduinoOTA.h`
- `WiFiManager.h`

## Project Setup

### 1. Upload the Code

1. Open this code in the **Arduino IDE**.
2. Install the necessary libraries from the Library Manager.
3. Configure the **Wi-Fi credentials** and **hostname** in the code:
   ```cpp
   const char* hostname = "sensor1";
   ```
4. Upload the code to your ESP32 board.

### 2. Connect to Wi-Fi

When the ESP32 boots:
- If it has previously saved Wi-Fi credentials, it will attempt to connect automatically.
- If not, the **WiFiManager** will open a captive portal (named **ESP32_AP**) where you can connect via a mobile or computer and input the Wi-Fi credentials.

### 3. Access the ESP32 with mDNS

Once the ESP32 is connected, you can access it using the mDNS hostname:
```bash
http://sensor1.local
```

> **Note**: Ensure that your computer/mobile device supports **mDNS** and is on the same Wi-Fi network as the ESP32.

### 4. Over-the-Air (OTA) Updates

#### Setting Up OTA
The code includes the necessary OTA configurations:
- Hostname for OTA updates.
- Password protection for OTA updates.

#### Performing an OTA Update
1. Use the Arduino IDE:
   - Go to **Tools > Port** and select your ESP32 via mDNS (e.g., `sensor1.local`).
   - Upload a new sketch wirelessly.

2. Use the ESP32 Web Updater (if you add it later) or other OTA methods.

### 5. Optional Deep Sleep Logic

You can implement deep sleep logic based on your project's requirements. The code includes an example to enter deep sleep after 5 minutes of inactivity:

```cpp
if (millis() > 300000) {
  Serial.println("Entering deep sleep...");
  ESP.deepSleep(600e6); // Sleep for 10 minutes
}
```

## How It Works
1. **WiFiManager** sets up the Wi-Fi connection.
2. **mDNS** service starts, allowing access via `http://sensor1.local`.
3. **OTA** is configured to handle remote firmware updates.
4. The ESP32 continuously monitors its free heap memory and prints it to the serial monitor.
5. Deep sleep logic can be implemented based on specific project requirements (optional).

## Code Explanation
- **Setup Function**: 
  - Initializes the serial communication.
  - Sets up **WiFiManager** to handle Wi-Fi connections.
  - Sets up mDNS with the specified hostname.
  - Configures and initializes OTA with password protection and callbacks.
- **Loop Function**: 
  - Handles OTA updates.
  - Monitors and prints free heap memory.
  - Optionally, implements deep sleep logic.

## Troubleshooting
1. **WiFi Connection Issues**:
   - Ensure the correct SSID and password are entered in the captive portal.
   - Reboot the ESP32 if it fails to connect.

2. **mDNS Not Working**:
   - Ensure your computer supports mDNS and is on the same network as the ESP32.
   - Try installing **Bonjour** on Windows if needed.

3. **OTA Update Failing**:
   - Ensure the ESP32 is connected to the same network as your computer.
   - Confirm the hostname and password settings in the code.

