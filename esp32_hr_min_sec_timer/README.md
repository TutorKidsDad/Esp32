ESP32 Countdown Timer project with a web interface using WiFiManager:
---
# ESP32 Countdown Timer with WiFiManager and Web Interface

This project is an ESP32-based countdown timer that displays the remaining time on an LCD. The timer duration can be set either via a web interface or in the code by default. It uses WiFiManager to handle Wi-Fi configuration through a captive portal.

## Features
- **Wi-Fi Configuration**: Uses WiFiManager to set up a Wi-Fi connection via a captive portal.
- **Web Interface**: Allows you to set the countdown timer duration via a webpage served by the ESP32.
- **LCD Display**: Displays the timer countdown and "Timer finished" message on completion.
- **Default Timer Value**: The default timer duration is set to 8 hours.

## Components Used
- ESP32 Development Board
- I2C LCD Display (16x2)

## Pin Configuration
| Component | ESP32 Pin |
|-----------|------------|
| LCD SDA   | GPIO 21    |
| LCD SCL   | GPIO 22    |

## Libraries Required
- **Wire**: For I2C communication.
- **LiquidCrystal_I2C**: For controlling the I2C LCD.
- **WiFiManager**: For easy Wi-Fi setup and configuration.
- **ESPAsyncWebServer**: To create the web server interface for setting the timer.
- **AsyncTCP**: For asynchronous web server support on ESP32.

## Project Setup

### 1. Hardware Connections
1. **Connect the LCD**:
   - Connect the LCD's SDA pin to GPIO 21 on the ESP32.
   - Connect the LCD's SCL pin to GPIO 22 on the ESP32.

### 2. Software Setup
1. **Install the Required Libraries**:
   - In the Arduino IDE, go to **Sketch** > **Include Library** > **Manage Libraries**. Search for and install the following:
     - **WiFiManager** by tzapu
     - **ESPAsyncWebServer** by Me No Dev
     - **AsyncTCP** by me-no-dev (for ESP32)
     - **LiquidCrystal_I2C** by Frank de Brabander

2. **Upload the Code**:
   - Connect your ESP32 to your computer.
   - Open the provided sketch in Arduino IDE.
   - Select the correct **Board** (ESP32) and **Port** from **Tools**.
   - Click **Upload** to flash the code to your ESP32.

### 3. Using the Countdown Timer
1. **Setting Up Wi-Fi**:
   - Upon powering up, the ESP32 will create a Wi-Fi access point named `Timer-Setup-AP`.
   - Connect to this Wi-Fi network with your smartphone or computer.
   - Once connected, a captive portal will open automatically. If not, navigate to `192.168.4.1` in a browser.
   - Choose and configure your Wi-Fi network credentials from the portal.

2. **Accessing the Web Interface**:
   - After successfully connecting to your Wi-Fi network, the ESP32 will automatically redirect you to the main page.
   - You can also find the IP address of the ESP32 by checking your router's connected devices.
   - Open a browser and enter the ESP32's IP address to access the timer configuration page.

3. **Setting the Timer via Web Page**:
   - On the webpage, enter the desired countdown time in hours and click **Set Timer**.

4. **LCD Countdown Display**:
   - The LCD will display the remaining time in `hh:mm:ss` format, updating every second.

5. **Completion Message**:
   - When the countdown ends, the LCD will display **"Timer finished"**.

## How It Works
- The ESP32 connects to a configured Wi-Fi network using WiFiManager.
- A web server is created using ESPAsyncWebServer, which serves a simple HTML form for setting the timer.
- The user can set a countdown time via the web page.
- The LCD displays the current countdown status, showing hours, minutes, and seconds remaining.
- When the countdown ends, the LCD displays "Timer finished."

## Project Modifications
Feel free to modify the project to suit your needs. You could:
- Change the default timer duration in the code.
- Modify the webpage to allow more precise control of the timer duration.
- Add additional features like pause/resume, reset, or different time increments for the web interface.

## Troubleshooting
- **Cannot connect to Wi-Fi**: Ensure that the ESP32 is in access point mode and that you are connecting to the correct SSID (`Timer-Setup-AP`).
- **Webpage not loading**: Make sure that your ESP32 is connected to your router. Find its IP address and enter it in the browser.
- **LCD not displaying correctly**: Verify the connections and ensure the LCD address is correct (`0x27` by default).

## Future Enhancements
- Add password protection to the captive portal.
- Implement additional features like pause/resume, reset, or configurable increments for timer settings.
- Add OTA (Over-the-Air) updates to update the firmware wirelessly.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

