ESP32-based Dust Sensor project, including the integration of WiFiManager and mDNS. The README file is structured to explain the purpose, setup instructions, and usage of the project.

# ESP32 Dust Sensor with WiFiManager and mDNS

## Project Overview
This project uses an ESP32 microcontroller to measure dust concentration using a dust sensor, send the measured data to a remote server, and provide an easy Wi-Fi configuration interface using **WiFiManager**. The project also uses **mDNS** to allow access to the ESP32 through a human-friendly hostname (`sensor1.local`) instead of an IP address.

## Features
- **WiFiManager Integration**: Automatically opens a captive portal to configure Wi-Fi if not already connected.
- **mDNS Support**: Access the ESP32 using a hostname (`http://sensor1.local`) on the local network.
- **Dust Sensor Data**: Measures dust concentration in µg/m³ and sends the data to a remote server.
- **Deep Sleep for Power Efficiency**: If the ESP32 fails to connect to Wi-Fi, it enters deep sleep to save power.

## Prerequisites
1. **Hardware Requirements**:
   - ESP32 microcontroller
   - Dust Sensor (e.g., GP2Y1010AU0F or similar)
   - Resistors and wiring components for interfacing
   - Optional: I2C LCD for local data display

2. **Software Requirements**:
   - **Arduino IDE** with the ESP32 board package installed
   - WiFiManager Library (`tzapu/WiFiManager`)
   - ArduinoJson Library
   - ESPmDNS Library

3. **Firebase Cloud or Other REST API**:
   - You need an endpoint URL to which the ESP32 sends the dust data. This example uses a Firebase URL.

## Wiring Diagram
Connect the dust sensor as follows:
- **Sensor LED control pin** to GPIO 12 on the ESP32.
- **Sensor analog output** to GPIO 36 (Analog input).
- Ensure that your sensor’s power and ground are properly connected to the ESP32’s 5V and GND, respectively.

## Getting Started

### Step 1: Uploading the Code to ESP32
1. Open the Arduino IDE and install the required libraries:
   - `WiFiManager` by Tzapu
   - `ArduinoJson` by Benoît Blanchon
   - `ESPmDNS`

2. In the code, customize the server URL and hostname as needed. The default hostname is `sensor1`.

3. Upload the provided code to your ESP32.

### Step 2: Setting Up the Wi-Fi Connection
1. Power up the ESP32. If there are no saved Wi-Fi credentials, it will create a Wi-Fi access point named **ESP32_Sensor**.
2. Connect to **ESP32_Sensor** with the default password `sensor123`.
3. Once connected, a captive portal will automatically open (or navigate to `192.168.4.1` if it doesn’t open automatically). 
4. Configure the Wi-Fi connection from the portal page.

### Step 3: Using mDNS
After the Wi-Fi connection is successful, the ESP32 will start mDNS with the hostname `sensor1`. You can access the ESP32 by typing `http://sensor1.local` in your browser.

### Step 4: Viewing Data on the Serial Monitor
Open the Serial Monitor (set baud rate to 115200) to view the dust concentration data and see if the ESP32 is successfully connecting to Wi-Fi and the server.

### Step 5: Monitoring Data on the Server
The ESP32 will periodically send the dust concentration data to the specified remote server (e.g., Firebase). Make sure your server is properly set up to receive HTTP PATCH requests with JSON payloads.

## Code Explanation
- **WiFiManager** is used to manage and store Wi-Fi credentials. It creates an access point if the ESP32 is unable to connect to Wi-Fi, allowing the user to input new Wi-Fi credentials.
- **mDNS** is set up to allow accessing the ESP32 by hostname (`sensor1.local`) rather than its IP address, simplifying connectivity within a local network.
- **HTTP Client** is used to send dust sensor data to a remote server endpoint.
- A **Moving Average Filter** is implemented to smooth the dust sensor’s analog readings for more accurate measurements.
- The code includes deep sleep functionality to save power when no Wi-Fi connection is available.

## Customizing the Code
- **Wi-Fi Credentials**: You can change the default captive portal SSID and password in the `setup()` function.
- **mDNS Hostname**: Change the `MDNS.begin("sensor1")` line to use a different hostname if needed.
- **Server URL**: Update the `serverURL` variable with your remote server endpoint.
- **Sensor Calibration**: Modify the conversion constants (`COV_RATIO`, `NO_DUST_VOLTAGE`, and `SYSTEM_VOLTAGE`) according to your sensor’s specifications.

## Troubleshooting
1. **Cannot Connect to ESP32 Wi-Fi Access Point**: Ensure the ESP32 is properly powered and the code has been uploaded without errors.
2. **mDNS Not Working**: Make sure your router supports mDNS and that your computer is on the same local network as the ESP32.
3. **No Data on the Server**: Check if the ESP32 is connecting to Wi-Fi and if the server URL is correctly set.

## License
This project is open-source and can be used, modified, and distributed freely.

