ESP32-based Countdown Timer project with WiFi configuration:


# ESP32 Countdown Timer with WiFiManager

This project is an ESP32-based countdown timer that allows you to set a countdown duration using physical buttons or a web interface. It includes an LCD to display the countdown status and a buzzer that sounds when the timer reaches zero. The project utilizes WiFiManager to handle Wi-Fi configuration through a captive portal.

## Features
- **Set Timer with Buttons**: You can increase or decrease the timer value using physical UP and DOWN buttons.
- **Web Interface Configuration**: Set the countdown duration through a web page served by the ESP32.
- **LCD Display**: Displays the timer status and value.
- **Buzzer Notification**: Alerts you when the countdown has finished.
- **WiFi Configuration**: Uses WiFiManager to set up a Wi-Fi connection.

## Components Used
- ESP32 Development Board
- I2C LCD Display (16x2)
- Push Buttons (for UP, DOWN, and START)
- Buzzer

## Pin Configuration
| Component   | ESP32 Pin |
|-------------|------------|
| Buzzer      | GPIO 23    |
| UP Button   | GPIO 17    |
| DOWN Button | GPIO 18    |
| START Button| GPIO 19    |
| LCD SDA     | GPIO 21    |
| LCD SCL     | GPIO 22    |

## Libraries Required
- **Wire**: For I2C communication.
- **LiquidCrystal_I2C**: For controlling the I2C LCD.
- **WiFiManager**: For easy Wi-Fi setup and configuration.
- **ESPAsyncWebServer**: To create the web server interface for setting the timer.

## Project Setup

### 1. Hardware Connections
1. **Connect the LCD**:
   - Connect the LCD's SDA pin to GPIO 21 on the ESP32.
   - Connect the LCD's SCL pin to GPIO 22 on the ESP32.
2. **Connect the Buttons**:
   - Connect UP Button to GPIO 17 with a pull-down or pull-up resistor.
   - Connect DOWN Button to GPIO 18 with a pull-down or pull-up resistor.
   - Connect START Button to GPIO 19 with a pull-down or pull-up resistor.
3. **Connect the Buzzer**:
   - Connect the positive terminal of the buzzer to GPIO 23.
   - Connect the negative terminal of the buzzer to the ESP32 ground (GND).

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
   - On the webpage, enter the desired countdown time in seconds and click **Set Timer**.

4. **Operating the Timer with Buttons**:
   - Press the **UP Button** to increase the timer value by 10 seconds.
   - Press the **DOWN Button** to decrease the timer value by 10 seconds (minimum value of 10 seconds).
   - Press the **START Button** to start the countdown.
   - Once started, the countdown value will be displayed on the LCD.

5. **Buzzer Notification**:
   - When the countdown reaches zero, the buzzer will sound, and "Time up!" will be displayed on the LCD.

## How It Works
- The ESP32 connects to a configured Wi-Fi network using WiFiManager.
- A web server is created using ESPAsyncWebServer, which serves a simple HTML form for setting the timer.
- The user can set a countdown time either through the physical buttons or the web page.
- The LCD displays the current countdown status and time remaining.
- When the countdown ends, a buzzer sounds to indicate completion.

## Project Modifications
Feel free to modify the project to suit your needs. You could:
- Change the timer increment values.
- Add additional buttons for finer control.
- Use a different type of display or notification mechanism.

## Troubleshooting
- **Cannot connect to Wi-Fi**: Ensure that the ESP32 is in access point mode and that you are connecting to the correct SSID (`Timer-Setup-AP`).
- **Webpage not loading**: Make sure that your ESP32 is connected to your router. Find its IP address and enter it in the browser.
- **Buttons not working**: Verify the connections and check if the input pins are set correctly.

## Future Enhancements
- Add password protection to the captive portal.
- Implement additional features like pause/resume, reset, or configurable increments for timer settings.
- Add OTA (Over-the-Air) updates to update the firmware wirelessly.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

