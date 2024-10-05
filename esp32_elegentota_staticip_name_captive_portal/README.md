# ESP32 WiFi Manager with Web Interface

This code is for an ESP32-based project that allows users to set and update WiFi credentials using a web interface. It provides functionalities like reading and saving WiFi credentials to EEPROM, connecting to a WiFi network, and setting up a captive portal (local web server) to enter new credentials if the connection fails.

## Detailed Explanation:

### 1. Libraries Used:
- `WiFi.h`, `HTTPClient.h`: For WiFi connectivity and HTTP communication.
- `WebServer.h`: To set up a local web server on the ESP32.
- `EEPROM.h`: To store and retrieve the WiFi credentials.
- `SPIFFS.h`: Used for file system handling (though not fully utilized in this code).
- `Arduino_JSON.h`: For handling JSON responses in the web interface.
- `ESPmDNS.h`: For resolving names instead of IP addresses (not used directly but included).

### 2. Global Variables:
- `i`, `statusCode`: Used for general counting and handling HTTP status codes.
- `ssid`, `passphrase`: Default SSID and password (currently not used).
- `esid`, `epass`: Strings to store the WiFi SSID and password read from EEPROM.
- `st`, `content`: Used to build the HTML content for the web server.
- `server`: Defines a web server on port 80.

### 3. Setup Function:
- **Serial Initialization**: Starts serial communication at 115200 baud.
- **WiFi Disconnect**: Disconnects from any existing WiFi connection.
- **EEPROM Initialization**: Initializes the EEPROM with a size of 512 bytes.
- **Pin 15 as Input**: Configures GPIO 15 as an input pin (used to check a button state).
- **Read WiFi Credentials from EEPROM**:
  - Reads stored SSID (first 32 bytes) and password (next 64 bytes) from EEPROM.
  - Attempts to connect to the stored WiFi credentials using `WiFi.begin()`.

### 4. Loop Function:
- **Check WiFi Connection**:
  - If connected to WiFi, it prints a confirmation message 10 times.
- **Button Press Check**:
  - It checks if GPIO 15 is not pressed (`digitalRead(15) != 1`).
  - If WiFi is connected, it returns and continues. Otherwise, it moves to set up a hotspot (Access Point).
- **Launch Hotspot**: If WiFi isn't connected or GPIO 15 is pressed, it starts a hotspot using `setupAP()` and `launchWeb()` to allow the user to input WiFi credentials.

### 5. `testWifi` Function:
- Waits up to 20 cycles (10 seconds total) for WiFi to connect.
- If connected, it returns `true`; otherwise, it times out and returns `false`, prompting the ESP32 to switch to Access Point mode.

### 6. `launchWeb` Function:
- Starts the web server and prints the local IP and AP (Access Point) IP addresses to the serial monitor.
- Calls `createWebServer()` to define the web pages for user interaction.

### 7. `setupAP` Function:
- Scans for nearby WiFi networks and lists them.
- Stores the network names (SSID) and their signal strength in an HTML list (`st`).
- Sets the ESP32 to Access Point mode (`WiFi.softAP()`), using "ElectronicsInnovation" as the SSID with no password.
- Reuses the `launchWeb()` function to start the web server for entering new credentials.

### 8. `createWebServer` Function:
- **Main Web Page ("/")**:
  - Displays an HTML form for entering WiFi credentials (SSID and password) and a list of nearby networks.
- **Scan Page ("/scan")**:
  - Currently a placeholder that prints "go back" and doesn't perform a network scan.
- **Setting Page ("/setting")**:
  - Accepts WiFi credentials submitted by the user (SSID and password).
  - Clears the EEPROM and saves the new credentials.
  - After saving, it reboots the ESP32 (`ESP.restart()`), allowing it to attempt connecting to the newly saved WiFi network.

### 9. How It Works:
- **WiFi Connection Attempt**: At startup, it reads the saved SSID and password from EEPROM and attempts to connect to the WiFi.
- **Hotspot Mode**: If it fails to connect to WiFi or GPIO 15 is pressed, the ESP32 starts an Access Point (AP) and serves a web page.
- **Credentials Update**: The web page allows the user to scan for networks and input new SSID and password. These credentials are saved to EEPROM and used for future WiFi connections.
- **Server Handling**: The ESP32 handles client requests on the local web server and updates the WiFi credentials based on user input.

---

This code essentially creates a self-contained WiFi manager, allowing users to easily update and save WiFi credentials on an ESP32 without having to reprogram the device.

