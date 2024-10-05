# ESP32 Web Server with PWM Slider Control

This project creates a simple web server on an ESP32 that allows users to control the brightness of an LED (or any other device connected to a GPIO pin) using a slider. Below is a detailed explanation of how it works:

## 1. Wi-Fi Connection Setup
- The ESP32 connects to a Wi-Fi network using the credentials (`ssid` and `password`) provided in the code.
- Once connected, the ESP32 prints its local IP address to the serial monitor. You can use this IP address to access the web server from a browser on the same network.

## 2. PWM Setup
- The ESP32's **PWM (Pulse Width Modulation)** functionality is used to control the brightness of an LED. PWM works by varying the duty cycle of a signal, which effectively controls the power supplied to the LED.
- The PWM settings are:
  - `freq = 5000` – The frequency of the PWM signal in Hz.
  - `ledChannel = 0` – The PWM channel used.
  - `resolution = 8` – The resolution of the PWM signal, meaning it can have 256 different levels (0-255).

## 3. Web Server
- The ESP32 acts as a web server on port 80, serving an HTML page that allows users to control the LED brightness with a slider.

## 4. HTML/CSS/JavaScript Interface
- The webpage (`index_html[]`) contains a slider, styled using CSS, that allows the user to set a value between 0 and 255.
- **JavaScript** in the webpage sends the slider value to the ESP32 via an AJAX request when the slider is adjusted.
- The slider value is displayed on the page and updates in real-time as you move the slider.

## 5. Processing Web Requests
- The `server.on("/")` function responds to HTTP GET requests at the root ("/") of the ESP32, serving the webpage defined in the `index_html[]`.
- The `server.on("/slider")` function responds to HTTP GET requests at `/slider?value=<inputMessage>`, where `<inputMessage>` is the value from the slider (ranging from 0 to 255).
  - The slider value is read from the URL and passed to the `ledcWrite()` function, which sets the brightness of the LED by updating the PWM duty cycle.

## 6. PWM Control
- The slider controls the PWM duty cycle of the pin (`output = 2`) attached to the LED.
- When the user moves the slider, the PWM value is updated, and the LED brightness changes accordingly.

## 7. Real-Time Slider Updates
- The JavaScript code sends the slider value to the ESP32 asynchronously using AJAX (`XMLHttpRequest`). This ensures the page doesn't reload every time the slider is adjusted, providing a smooth user experience.

## Overall Workflow
1. The ESP32 connects to the Wi-Fi network.
2. The user accesses the ESP32's IP address in a web browser.
3. The HTML page is displayed with a slider.
4. The user adjusts the slider, which sends the value to the ESP32.
5. The ESP32 updates the PWM signal on pin 2, changing the brightness of the connected LED.

This project demonstrates the use of the ESP32's web server capabilities combined with its hardware PWM functionality to create an interactive control system.

