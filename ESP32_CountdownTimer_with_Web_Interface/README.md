
# ESP32 Countdown Timer with Web Interface

This project sets up an ESP32 as a Wi-Fi Access Point (AP) that provides a mobile-friendly web interface for controlling a countdown timer. Users can connect to the ESP32’s network and access the timer page using mDNS at `http://timer.local`. From the web interface, users can start/stop the timer, and adjust the countdown time with "Up" and "Down" buttons.

## Features
- **Standalone Wi-Fi Access Point**: The ESP32 hosts its own network, allowing devices to connect directly.
- **mDNS Support**: Users can access the timer interface via `http://timer.local` without needing the ESP32’s IP address.
- **Mobile-Friendly Web Interface**: The embedded HTML interface allows users to start/stop the timer and adjust the countdown time with a simple button interface.
- **Real-Time Countdown Display**: The remaining time is updated every second in the browser.

## Hardware Requirements
- **ESP32 Development Board**
- **Momentary Push Buttons** (3 buttons for start/stop, increase, and decrease countdown time)
- **Wiring**:
  - `buttonPin` (Pin 19): Start/Stop button
  - `upButtonPin` (Pin 17): "Up" button to increase countdown time
  - `downButtonPin` (Pin 18): "Down" button to decrease countdown time

## Software Requirements
- **Arduino IDE** with ESP32 board support
- **Libraries**:
  - [WiFi.h](https://www.arduino.cc/en/Reference/WiFi): Included with ESP32 board setup in Arduino IDE.
  - [ESPmDNS.h](https://github.com/espressif/arduino-esp32/tree/master/libraries/ESPmDNS): For mDNS support.

## Setup and Usage

### 1. Wiring
Connect three push buttons to the ESP32:
- **Start/Stop** button to Pin 19
- **Up** button to Pin 17
- **Down** button to Pin 18
Each button should be connected to ground with an **INPUT_PULLUP** configuration in code.

### 2. Flash the Code
1. Open the Arduino IDE.
2. Copy the code from `countdown_timer.ino` into the IDE.
3. Select the correct board and port for your ESP32.
4. Click **Upload** to flash the code onto the ESP32.

### 3. Connect to the ESP32 Network
1. After flashing, the ESP32 will start an access point with the following credentials:
   - **SSID**: `ESP32_Timer_Control`
   - **Password**: `12345678`
2. Connect to this network from your mobile device or computer.

### 4. Access the Timer Interface
- Open a web browser and go to `http://timer.local`.
- You should see a web interface with buttons to **Start/Stop**, **Increase** (Up), and **Decrease** (Down) the countdown timer.

## Web Interface Overview
- **Start/Stop**: Toggle the countdown timer.
- **Up**: Increases the countdown time by 1 second.
- **Down**: Decreases the countdown time by 1 second (minimum of 1 second).
- **Time Left Display**: Shows the remaining time in seconds, updated every second.

## Code Overview
- **Web Server**: Sets up routes for handling button actions from the web interface.
- **mDNS Setup**: Configures mDNS so users can access the page via `http://timer.local`.
- **Countdown Timer Logic**: Starts and stops the countdown timer and adjusts time based on button inputs from either hardware buttons or the web interface.

## Troubleshooting
1. **Unable to connect to `http://timer.local`**:
   - Ensure your device is connected to the ESP32's network.
   - Check that mDNS is enabled on your device. For some systems, mDNS might not work by default.

2. **Timer does not update**:
   - Confirm that the web page is refreshing the countdown time by checking the serial monitor for updates.

## License
This project is open-source and available for modification and distribution under the MIT License.

---

This `README.md` should provide a complete overview of the project, installation, and usage instructions for users looking to set up the ESP32 countdown timer with mDNS access.
