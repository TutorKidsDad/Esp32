
# ESP Web Server Project

## Description
This project creates a web server using the ESP32 or ESP8266 microcontroller, allowing users to control an output (like an LED) and set a timer using a web-based interface. The server serves an HTML page that includes a toggle switch and a slider to control the output state and timer duration, respectively.

## Author
- **Modified by**: Swapnil Karambelkar
- **Original Author**: Rui Santos  
- **Project Details**: [Random Nerd Tutorials](https://randomnerdtutorials.com)

## Features
- Connect to a Wi-Fi network as an access point.
- A web interface to toggle an output state.
- A slider to set a timer value.
- Real-time updates on the web page without refreshing.

## Required Libraries
- **ESP32**: `WiFi.h`, `AsyncTCP.h`, `ESPAsyncWebServer.h`
- **ESP8266**: `ESP8266WiFi.h`, `ESPAsyncTCP.h`, `ESPAsyncWebServer.h`

## Hardware Requirements
- ESP32 or ESP8266 microcontroller.
- An LED or any output device connected to GPIO pin 2 (or modify the code for a different pin).

## Installation
1. **Clone the repository** or download the code into the Arduino IDE.
2. **Install required libraries** via the Library Manager:
   - ESPAsyncWebServer
   - ESPAsyncTCP (only for ESP8266)
3. **Connect the hardware**:
   - Connect an LED (or output device) to GPIO pin 2.
4. **Upload the code** to your ESP32/ESP8266 using the Arduino IDE.

## Configuration
1. Replace the following with your network credentials in the code:
   ```cpp
   const char* ssid     = "Access-Point";
   const char* password = "123456789";
   ```
2. Optionally, change the `output` pin if necessary:
   ```cpp
   const int output = 2;
   ```

## How to Use
1. After uploading the code, open the Serial Monitor (set to 115200 baud) to see the access point's IP address.
2. Connect to the ESP access point using your device's Wi-Fi settings.
3. Open a web browser and go to `http://<AP_IP>` (usually `192.168.4.1`).
4. Use the toggle switch to turn the output on and off.
5. Use the slider to set the timer duration.

## Code Overview
- The `index_html` variable contains the HTML content for the web interface, including styles and JavaScript for functionality.
- The web server listens for HTTP requests and updates the output state or slider value accordingly.
- The `processor` function handles the dynamic content replacement in the HTML.

## Troubleshooting
- Ensure the necessary libraries are installed and up to date.
- If the device does not connect to the access point, check the Wi-Fi credentials.
- Check the Serial Monitor for error messages or connection issues.

## License
This project is open-source. Feel free to modify and use it for your own purposes.


