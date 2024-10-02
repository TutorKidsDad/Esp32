# ESP32 Web Server PWM Control

This project demonstrates how to create a web server on an ESP32 or ESP8266 that allows users to control the Pulse Width Modulation (PWM) of an LED or motor using a slider on a web interface.

## Features
- Control PWM output using a web slider.
- Responsive and modern web interface.
- Real-time PWM value display.
- WiFi-enabled control through your local network.

## Requirements
- ESP32 or ESP8266 development board.
- Servo motor or an LED for PWM output (connected to GPIO pin 2 by default).
- Power supply (via USB or battery).
- A local WiFi network for the ESP to connect to.

## Installation
### 1. Install the Required Libraries
Ensure you have the following libraries installed in your Arduino IDE:
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)

You can install these libraries using the Arduino Library Manager or manually download them from GitHub.

### 2. Upload the Code
- Open the provided code in the Arduino IDE.
- Update the WiFi credentials:
  ```cpp
  const char* ssid = "your-SSID";
  const char* password = "your-PASSWORD";
## Instructions

### 3. Select Your Board

- Select your board (ESP32 or ESP8266) in the Arduino IDE.
- Connect your ESP board to the computer.
- Upload the code.

### 4. Wiring

| Pin        | ESP32/ESP8266   | Servo Motor/LED |
|------------|-----------------|-----------------|
| VCC        | 3.3V or 5V      | VCC             |
| GND        | GND             | GND             |
| PWM Signal | GPIO 2 (Default) | Signal Pin      |

> **Note:** You can modify the pin for your project by changing the value of the `output` variable in the code.

### 5. Web Interface

Once the code is uploaded and the device is connected to WiFi:
- Open the Serial Monitor to get the IP address assigned to your ESP board.
- You can access the control interface via this IP address on any device connected to the same network.

**Example:**

http://<your-esp-ip-address>

#### Web Interface Features:

- **Slider:** Control the PWM signal by adjusting the slider.
- **Real-time Display:** The current PWM value is displayed as you adjust the slider.

### 4. Circuit Diagram

Here is a simple wiring diagram for connecting the components:
    ESP32/ESP8266
   +---------------+
   |               |
   |     3.3V/5V --+----> VCC
   |        GND ----+----> GND
   |   GPIO 2 (PWM) ----> Signal Pin (Servo/LED)
   +---------------+

### 5. Example Usage

After successfully uploading the code and connecting to WiFi:

1. Open the web interface by entering the ESP's IP address in a browser.
2. Move the slider to adjust the PWM value (0 - 255). The change is reflected in real-time.
3. The PWM signal controls the brightness of an LED or the position of a servo motor.

### Future Improvements

- **Multiple Controls:** Add more sliders or buttons to control multiple outputs.
- **Real-time Feedback:** Display real-time sensor data or other inputs on the web interface.
- **Authentication:** Implement a login system to restrict access to the control interface.
- **Mobile Optimization:** Further optimize the web interface for mobile devices.


