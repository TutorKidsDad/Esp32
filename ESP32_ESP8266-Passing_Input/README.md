# ESP32/ESP8266 Web-Based Servo Controller with Colorful Interface

This project demonstrates how to create a web-based interface for controlling servos or collecting user inputs using an ESP32 or ESP8266 microcontroller. The web interface includes colorful, modern HTML forms for entering string, integer, and float values, which can then be processed by the ESP to control hardware like servos.

## Features

- **ESP32/ESP8266 WiFi Connectivity**: Connects the ESP32/ESP8266 to a local WiFi network to enable remote control via a web interface.
- **Colorful HTML Form**: A visually appealing, responsive web page with input fields for string, integer, and float values.
- **Non-blocking Asynchronous Web Server**: Ensures smooth performance even with multiple requests.
- **Error Handling**: 404 pages are handled gracefully.
- **Serial Monitoring**: Input values can be viewed in the serial monitor for debugging.

## Required Components

- **ESP32 or ESP8266 microcontroller**.
- Servo motor (optional, for testing).
- WiFi connection for web-based control.

## Required Libraries

Ensure you have the following libraries installed in the Arduino IDE:

- For ESP32:
  - `WiFi.h`
  - `AsyncTCP.h`
  - `ESPAsyncWebServer.h`
- For ESP8266:
  - `ESP8266WiFi.h`
  - `ESPAsyncTCP.h`
  - `ESPAsyncWebServer.h`

To install these libraries, go to **Sketch** -> **Include Library** -> **Manage Libraries...**, and search for them.

## Getting Started

### 1. Setup

- Clone this repository or download the `.ino` file and open it in the Arduino IDE.
- Replace the following lines with your WiFi network credentials:
  ```cpp
  const char* ssid = "your_SSID";
  const char* password = "your_PASSWORD";
## Upload the Code

Upload the code to your ESP32 or ESP8266 board.

## Wiring

- **VCC**: Connect to 3.3V or 5V (depending on your microcontroller).
- **GND**: Connect to ground.
- **Servo Motor (Optional)**: Connect to an appropriate GPIO pin if you’re testing servo control.

## Web Interface

Once the code is uploaded and the device is connected to WiFi, open the Serial Monitor to get the IP address. You can then open this address in a web browser to view the control page.

The web interface consists of the following elements:

- **String Input**: Allows the user to send a string to the ESP.
- **Integer Input**: Accepts an integer value.
- **Float Input**: Accepts a floating-point number.

## HTML Form Preview

The form is designed with modern CSS for better aesthetics. It includes hover effects, vibrant colors, and responsiveness for mobile devices.

### Example Web Form Layout

Here’s how the form looks:

- **String Input**: Input text for sending custom messages.
- **Integer Input**: Useful for inputting numerical data (like servo angles).
- **Float Input**: Allows more precise control over servo positions or other parameters.

## Serial Monitor

Input values from the web interface are displayed in the Serial Monitor. You can extend the functionality by adding servo or sensor controls based on the received values.

## Example Usage

After connecting the ESP to WiFi and opening the web interface, you can enter values into the fields and submit them. The form sends an HTTP GET request to the ESP, which processes the inputs and returns a message showing the entered values.

Example URL after submitting string input:
http://<ESP_IP_ADDRESS>/get?HTML_STR_INPUT1=HelloWorld


## Error Handling

If a user tries to access a non-existent route or enters an incorrect URL, the server responds with a 404 error and a custom "Not Found" message.

## Circuit Diagram

Here’s a simple connection diagram:

- **ESP32/ESP8266**: Connect to a USB power source.
- **Servo Motor (Optional)**: Connect the signal pin to a GPIO pin, VCC to 5V, and GND to ground.

## Future Improvements

- **Servo Control**: Implement servo control based on the integer and float inputs.
- **Real-time Data**: Display real-time sensor data or servo positions on the web interface.
- **Additional Input Elements**: Add sliders, dropdowns, or other HTML elements for more advanced control.



