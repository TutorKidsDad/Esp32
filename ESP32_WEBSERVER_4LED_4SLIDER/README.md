## Project Overview: ESP32 Web-Based LED Control with PWM

This project uses an ESP32 microcontroller to create a web server that allows users to control the state (ON/OFF) and brightness of four LEDs through a web interface. The ESP32 sets up a Wi-Fi network, and users can connect to it and control the LEDs from any web browser.

### Key Features
1. **Wi-Fi Setup**: The ESP32 acts as an Access Point (AP) with the SSID `REMOTE-CONTROL` and password `12345678`.
2. **LED On/Off Control**: The web interface allows toggling the state of four LEDs connected to GPIO pins.
3. **PWM Control**: The brightness of the LEDs can be adjusted using Pulse Width Modulation (PWM) sliders on the web interface.
4. **Real-Time Control**: Changes in LED status and brightness are reflected in real-time.

### Components
- **4 LEDs**: Connected to GPIO pins 15, 2, 4, and 5 for ON/OFF control.
- **4 PWM channels**: Connected to GPIO pins 18, 19, 21, and 22 for brightness control.

### Hardware Setup
1. **LED On/Off Pins**: The four LEDs are connected to the following pins:
   - LED 1: GPIO 15
   - LED 2: GPIO 2
   - LED 3: GPIO 4
   - LED 4: GPIO 5

2. **PWM Channels**: Four additional GPIO pins (18, 19, 21, and 22) are used to control the brightness of each LED via PWM.

### Code Breakdown

#### 1. **Wi-Fi Access Point Setup**
- The ESP32 is configured to create a local Wi-Fi network with the given SSID and password. This enables other devices to connect to it.
- The device uses a fixed IP address (`192.168.1.1`) and subnet configuration to act as a web server.

#### 2. **Web Server Setup**
- The project uses the `WebServer` library to host a simple HTTP server on port 80. The server listens for incoming requests and serves the HTML page where users can control the LEDs.

#### 3. **Routes for LED Control**
- The server has routes for turning LEDs on and off, for example:
  - `/Switch1on` turns LED 1 on.
  - `/Switch1off` turns LED 1 off.
- Each route updates the corresponding LED state and reloads the web interface to reflect the changes.

#### 4. **Routes for PWM Control**
- Routes such as `/PWM1` handle changes to the brightness of LEDs. The PWM value is passed as a URL parameter, and the server updates the duty cycle for the respective LED.
- Each LED has its own slider to control brightness, ranging from 0 (off) to 255 (full brightness).

#### 5. **LED Status and Brightness**
- The web interface shows the current status (ON/OFF) of each LED and provides a slider to adjust its brightness.
- The interface dynamically sends requests back to the ESP32 server to update the LED status and PWM values.

#### 6. **PWM Configuration**
- The code configures four separate PWM channels with a frequency of 5000 Hz and 8-bit resolution.
- The brightness of each LED is controlled by varying the duty cycle of the respective PWM channel.

#### 7. **HTML Web Interface**
- The `SendHTML` function generates the HTML code that displays on the user’s web browser.
  - **Buttons**: Toggle buttons to switch LEDs ON or OFF.
  - **Sliders**: Input sliders to control LED brightness using PWM.

#### 8. **JavaScript for Real-Time Control**
- The HTML includes JavaScript functions that handle real-time updates to the PWM values as users move the sliders. This sends the new values to the server asynchronously, ensuring a smooth user experience.

### How It Works:
1. **Startup**: The ESP32 creates a Wi-Fi network and starts the web server.
2. **User Connection**: A user connects to the network and navigates to the ESP32's IP address (`192.168.1.1`) in their browser.
3. **Web Interface**: The user is presented with buttons to toggle the LEDs and sliders to adjust brightness.
4. **Control**: When the user interacts with the interface, the ESP32 handles the corresponding request, updating the LEDs' states and/or PWM values in real-time.

### Conclusion
This project provides a simple yet powerful way to control LEDs using an ESP32 over a local Wi-Fi network. The combination of on/off control and brightness adjustment through PWM makes it a versatile tool for LED-based projects or home automation systems.
