# ESP32 Web Server for LED Control and PWM Intensity

## Overview
This project demonstrates how to create a web server using the **ESP32**. The server allows you to control two LEDs remotely using an HTML-based web interface. You can turn each LED ON/OFF and adjust the brightness using a PWM slider.

### Features:
- Control two LEDs (ON/OFF) via a simple web interface.
- Adjust the brightness of each LED using a slider (PWM control).
- The web server runs in Access Point (AP) mode, allowing you to connect directly to the ESP32’s network and control the LEDs.

## Components Used
- **ESP32**: The main microcontroller running the web server and controlling the LEDs.
- **LEDs**: Two LEDs are connected to GPIO pins for control.
- **WiFi**: The ESP32 creates a local WiFi network that you can connect to.
  
## Hardware Setup
- **LED1**: Connected to GPIO4.
- **LED2**: Connected to GPIO5.
- Ensure proper resistors are used in series with the LEDs to avoid damage.

## Software Requirements
- **Arduino IDE** with ESP32 board support installed.
- Libraries:
  - `WiFi.h`: For creating the Access Point and handling WiFi connections.
  - `WebServer.h`: For setting up the web server and handling requests.

## Web Interface
The web interface allows you to:
- Turn **LED1** ON or OFF.
- Turn **LED2** ON or OFF.
- Adjust the brightness of **LED1** and **LED2** using sliders (PWM control).
- The current status of each LED (ON/OFF) is displayed dynamically.

## How It Works
1. **Access Point Creation**: 
   - The ESP32 creates its own WiFi network with the SSID `REMOTE-CONTROL` and password `12345678`.
   - Once connected to this network, users can access the web interface through the IP `192.168.1.1`.
   
2. **Web Server**:
   - A web server runs on the ESP32 and listens for incoming HTTP requests.
   - Different routes (`/Switch1on`, `/Switch1off`, `/Switch2on`, `/Switch2off`, and `/Switch1?pwm=`) handle LED control and PWM adjustments.

3. **PWM Control**:
   - The brightness of the LEDs can be adjusted using a range slider that sends a PWM value (0-255) to the ESP32, which then applies the appropriate duty cycle to control LED intensity.

## Code Explanation

### Key Sections:

1. **WiFi Setup**:
   - The ESP32 creates a local Access Point with the SSID and password you specify in the code. This allows clients to connect directly to the ESP32 without the need for a separate router.

    ```cpp
    WiFi.softAP(ssid, password);
    WiFi.softAPConfig(local_ip, gateway, subnet);
    ```

2. **Web Server**:
   - Routes are set up to handle incoming HTTP requests. For instance, the route `/Switch1on` turns **LED1** ON, and `/Switch1off` turns it OFF.

    ```cpp
    server.on("/", handle_OnConnect);
    server.on("/Switch1on", handle_Switch1on);
    server.on("/Switch1off", handle_Switch1off);
    server.on("/Switch2on", handle_Switch2on);
    server.on("/Switch2off", handle_Switch2off);
    ```

3. **Handling Requests**:
   - The request handlers (`handle_Switch1on()`, `handle_Switch1off()`, etc.) change the state of the LEDs based on user input.
   
   - PWM control is handled through the `updateSlider1()` and `updateSlider2()` JavaScript functions in the HTML code.

    ```cpp
    void handle_Switch1on() {
      LED1status = HIGH;
      server.send(200, "text/html", SendHTML(true, LED2status));
    }

    void handle_Switch1off() {
      LED1status = LOW;
      server.send(200, "text/html", SendHTML(false, LED2status));
    }
    ```

4. **HTML Code**:
   - The HTML page is generated dynamically and sent to the client. It includes buttons for controlling the LEDs and sliders for adjusting brightness.

    ```cpp
    String SendHTML(uint8_t led1stat, uint8_t led2stat) {
      String ptr = "<!DOCTYPE html> <html>\n";
      ptr += "<head> ... </head>";
      ptr += "<body> ... </body>";
      return ptr;
    }
    ```

5. **PWM Control**:
   - The sliders send the PWM value to the ESP32 using JavaScript, allowing for real-time brightness adjustment of the LEDs.

    ```html
    <input type="range" class="slider" id="led1" min="0" max="255" value="0" onchange="updateSlider1(this.value)">
    ```

    ```cpp
    void handle_Slider1PWM() {
      String pwmValue = server.arg("pwm");
      analogWrite(LED1pin, pwmValue.toInt());
    }
    ```

## How to Use
1. **Flash the code**: Upload the sketch to your ESP32 using the Arduino IDE.
2. **Connect to WiFi**: On your phone or computer, connect to the ESP32’s WiFi network (`REMOTE-CONTROL`).
3. **Control LEDs**: Open a browser and navigate to `192.168.1.1`. You'll see a web page where you can control the LEDs' ON/OFF status and adjust their brightness.

## License
This project is open-source and free to use for personal and educational purposes.


