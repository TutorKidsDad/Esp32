
# ESP32 WebSocket Sliders with WiFiManager

This project uses an ESP32 microcontroller to host a web server with WebSocket communication to control three PWM-controlled LEDs through a web interface. The project incorporates the **WiFiManager** library to simplify network configuration.

## Project Overview

This project demonstrates the use of ESP32 to:

- Host a web server that serves a web page allowing users to control three LEDs with sliders.
- Use WebSocket for real-time communication between the web page and ESP32.
- Store files using LittleFS for efficient storage and serving of web assets.
- Integrate **WiFiManager** for seamless network configuration using a captive portal.

## Hardware Requirements

- ESP32 Development Board
- Three LEDs with appropriate resistors
- Breadboard and connecting wires

### GPIO Pin Assignments

- **LED 1:** GPIO 12
- **LED 2:** GPIO 13
- **LED 3:** GPIO 14

## Software Requirements

- **Arduino IDE** (or PlatformIO) with the following installed:
  - **ESP32 Board Package**
  - **WiFiManager** library
  - **ESPAsyncWebServer** library
  - **AsyncTCP** library
  - **LittleFS** library
  - **Arduino_JSON** library

## Installation and Setup

1. **Clone or Download the Project**  
   Clone this repository or download the ZIP file and extract it into your Arduino sketch folder.

2. **Install Necessary Libraries**  
   Open the Arduino IDE and install the required libraries using the **Library Manager** (`Sketch > Include Library > Manage Libraries...`):
   - `WiFiManager`
   - `ESPAsyncWebServer`
   - `AsyncTCP`
   - `LittleFS`
   - `Arduino_JSON`

3. **Configure LittleFS for File Storage**  
   The web server serves the control interface from the ESP32's filesystem. Before uploading the sketch, ensure you have the **LittleFS Plugin** installed in your Arduino IDE to upload the HTML files.

4. **Upload Files to LittleFS**  
   - Place your `index.html` file in a folder named `data` within your project directory.
   - Use the **LittleFS Data Upload Tool** to upload the contents of the `data` folder to the ESP32.

5. **Upload the Code to ESP32**  
   Connect your ESP32 board to your computer and select the appropriate board and port from the **Tools** menu. Then, upload the sketch to the ESP32.

6. **Configure Wi-Fi Credentials**  
   - On the first boot, the ESP32 will create a Wi-Fi access point named `ESP32-Config-Portal`. 
   - Connect to this Wi-Fi network using your computer or mobile device. It should automatically redirect you to a captive portal page.
   - In the captive portal, select your Wi-Fi network and enter the password.

7. **Access the Web Interface**  
   After successful connection, the ESP32 will display the IP address in the serial monitor. Use this IP address in a browser to access the web interface.

## How It Works

- The web server serves an HTML page with three sliders representing each LED.
- Moving a slider sends real-time updates to the ESP32 using WebSocket.
- The ESP32 updates the corresponding PWM duty cycle, adjusting the LED brightness.
- If Wi-Fi credentials are not configured or if the ESP32 fails to connect to the saved Wi-Fi, it opens a captive portal using **WiFiManager**.

## File Structure

```
ESP32_WebSocket_Project/
│
├── ESP32_WebSocket_Project.ino   // Main project code
├── README.md                     // Project documentation (this file)
└── data/                         // LittleFS folder for web files
    └── index.html                // Web page to control the LEDs
```

## Dependencies

- **WiFiManager** - Handles Wi-Fi connection and configuration.
- **ESPAsyncWebServer** - Allows asynchronous HTTP server and WebSocket communication.
- **LittleFS** - Provides a lightweight file system for serving web files.
- **Arduino_JSON** - JSON handling library to manage slider values.

## License

This project is based on the work by Rui Santos & Sara Santos from [Random Nerd Tutorials](https://RandomNerdTutorials.com).  
The project and code are released under the MIT license.

## Credits

- **WiFiManager Library:** Tzapu & Ken Taylor
- **ESPAsyncWebServer Library:** Me-No-Dev
- **AsyncTCP Library:** Me-No-Dev

## Author

**Updated and Modified by Swapnil Karambelkar**

---
