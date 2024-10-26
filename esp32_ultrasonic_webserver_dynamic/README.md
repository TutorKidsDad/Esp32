# ESP32 Smart Height Measurement Project

This project utilizes an ESP32 microcontroller to measure distances using an ultrasonic sensor and displays the results via a web server. It uses the [WiFiManager](https://github.com/tzapu/WiFiManager) library to simplify Wi-Fi configuration.

## Features
- **Distance Measurement**: Measures distances using an ultrasonic sensor (HC-SR04).
- **Web Server**: Hosts a web page that displays the measured distance.
- **Wi-Fi Configuration**: Uses WiFiManager to allow users to set Wi-Fi credentials without hardcoding them into the code.

## Components Required
- ESP32 development board
- Ultrasonic sensor (HC-SR04)
- Breadboard and jumper wires

## Installation
1. **Hardware Setup**:
   - Connect the `trigPin` of the HC-SR04 sensor to GPIO 5 of the ESP32.
   - Connect the `echoPin` of the HC-SR04 sensor to GPIO 18 of the ESP32.
   - Provide appropriate power (5V and GND) connections to the sensor.

2. **Software Setup**:
   - Install the [Arduino IDE](https://www.arduino.cc/en/software) if not already done.
   - Install the ESP32 board support package. You can find instructions [here](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html).
   - Install the required libraries in Arduino IDE:
     - **WiFiManager**: Go to `Sketch -> Include Library -> Manage Libraries`, then search for and install **WiFiManager** by `tzapu`.

3. **Code**:
   - Copy the provided code into a new Arduino sketch.

4. **Upload the Code**:
   - Connect the ESP32 to your computer and select the appropriate board and port from the Arduino IDE.
   - Click on the "Upload" button to upload the code.

## How to Use
1. **Power Up**:
   - Power up the ESP32. If this is the first time, or if it cannot connect to a saved network, the ESP32 will create an access point (AP) called **AutoConnectAP**.
   
2. **Connect to the Access Point**:
   - Use a mobile device or computer to connect to the **AutoConnectAP** network.
   - Once connected, a captive portal should open automatically, allowing you to select your Wi-Fi network and enter the credentials.

3. **Access the Web Server**:
   - Once the ESP32 is connected to your Wi-Fi network, it will print the IP address to the serial monitor.
   - Open a web browser and enter the ESP32’s IP address to access the web page, which displays the measured distance.

## Code Explanation
### Libraries
- **WiFi.h**: Handles Wi-Fi connections.
- **WebServer.h**: Sets up an HTTP web server.
- **WiFiManager.h**: Allows configuration of Wi-Fi credentials without hardcoding them.

### Setup Function
- Configures the ultrasonic sensor pins and initializes the WiFiManager to establish a Wi-Fi connection.
- Starts a web server and sets up the route handlers.

### Loop Function
- Periodically measures the distance and handles incoming web requests.

### HTML Response
- A simple HTML page is served, displaying the measured distances in centimeters and inches.

## Future Improvements
- Add more sensors or features to expand the measurement capabilities.
- Include additional options or settings on the web interface.
- Implement security features, such as a password-protected captive portal.

## Troubleshooting
- **Cannot connect to the access point**: Ensure the ESP32 is properly powered and try resetting the device.
- **Unable to find the IP address**: Check the serial monitor for the printed IP address. Ensure your computer or mobile device is connected to the same network as the ESP32.

## References
- [ESP32 Board Package Installation](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
- [WiFiManager Library GitHub](https://github.com/tzapu/WiFiManager)

