# ESP32 IoT Project with AWS, OTA Updates, and Captive Portal

This project demonstrates how to create an IoT system using an ESP32 microcontroller, featuring Over-the-Air (OTA) updates, sensor data reading, and integration with AWS for data posting and retrieval. The system includes a captive portal for Wi-Fi configuration and can be used for remote sensor monitoring.

## Features
- **AWS IoT Integration**: Posts sensor data (temperature, humidity, and analog input) to AWS IoT and retrieves data from the AWS IoT shadow.
- **OTA Updates**: Allows the ESP32 to update its firmware over-the-air by checking the latest version available on a remote server.
- **Wi-Fi Captive Portal**: Simplifies Wi-Fi setup through a captive portal, so you don't need to hard-code Wi-Fi credentials.
- **DHT11 Sensor**: Reads temperature and humidity values using the DHT11 sensor.
- **Analog Sensor**: Reads an analog input from pin 34 of the ESP32.
- **LCD Display**: Displays sensor data and system status messages on a 16x2 I2C LCD.
- **Deep Sleep Mode**: Puts the ESP32 into deep sleep mode between measurements to conserve power.

## Components Required
- **ESP32 microcontroller**
- **DHT11 sensor**
- **16x2 I2C LCD Display**
- **AWS IoT account**
- **Wi-Fi access**

## Libraries Required
Make sure you install the following libraries in the Arduino IDE before compiling the project:
- `WiFiManager` - Handles the Wi-Fi captive portal.
- `HTTPClient` - Used for HTTP communication.
- `WiFiClientSecure` - For secure connection to AWS.
- `ArduinoJson` - For parsing JSON responses from AWS IoT.
- `DHT` - For reading from the DHT11 sensor.
- `LiquidCrystal_I2C` - For controlling the I2C LCD display.

## How It Works

### 1. Setup Phase
When the ESP32 is powered on, it performs the following steps:
- **Sensor Initialization**: The DHT11 sensor is initialized for temperature and humidity readings.
- **LCD Display Initialization**: The device name and status messages are displayed on the LCD.
- **Wi-Fi Connection**: The ESP32 attempts to connect to Wi-Fi using the captive portal. If the connection fails within 3 minutes, the ESP32 restarts.
- **OTA Update Check**: If connected to Wi-Fi, the device checks for any available OTA firmware updates. If a new version is available, it downloads and installs the update.

### 2. Main Loop
In the main loop, the ESP32:
- **Reads Analog Sensor Data**: Reads the analog value from pin 34.
- **Reads Temperature and Humidity**: Gathers data from the DHT11 sensor.
- **Posts Data to AWS IoT**: Posts the temperature, humidity, and analog values to AWS IoT.
- **Retrieves Data from AWS IoT**: Retrieves and prints sensor data from the AWS IoT shadow.
- **Deep Sleep**: After each loop, the ESP32 goes into deep sleep for 5 minutes to conserve power.

### 3. OTA Updates
The ESP32 checks for OTA updates by contacting a specified server URL to retrieve the latest firmware version. If the current version is outdated, it downloads and installs the new firmware, followed by a reboot.

### 4. Deep Sleep Mode
After completing sensor data collection and posting to AWS, the ESP32 enters deep sleep mode to save power. It wakes up after 5 minutes to repeat the process.

## AWS IoT Configuration

### 1. Set Up AWS IoT
To configure AWS IoT for this project:
1. **Create an IoT Thing** on AWS IoT Core.
2. Download the required certificates and attach them to the IoT Thing.
3. Set up an IoT policy that allows the ESP32 to publish and retrieve data.
4. Update the following fields in `aws_post.h` and `aws_read.h` with your AWS details:
   - `aws_endpoint`: Your AWS IoT endpoint.
   - `aws_path`: The path to the IoT Thing's shadow or MQTT topic.
   - `aws_key`: Your AWS API key or credentials for accessing the IoT service.

### 2. OTA Update Setup
To set up OTA updates:
1. Host your firmware on a remote server.
2. Modify the URLs in `ota_update.h` to point to your hosted firmware:
   ```cpp
   const char* version_url = "http://your-server.com/version.txt";
   const char* firmware_url = "http://your-server.com/firmware.bin";
## Pin Configuration

### DHT11 Sensor
- Data pin of the DHT11 is connected to **pin 14** of the ESP32.

### I2C LCD Display
- **SCL (Clock)** is connected to **pin 22**.
- **SDA (Data)** is connected to **pin 21**.

### Analog Input
- The analog sensor is connected to **pin 34** of the ESP32.

### Pin Definitions
Pin configuration is handled in the `pins.h` file:


#define DHTPIN 14    // Pin for DHT11 data
#define SCL 22       // I2C clock pin
#define SDA 21       // I2C data pin
#define ANALOG_PIN 34 // Pin for analog sensor

## Code Overview

### Key Files
- `ota_update.h`: Handles OTA updates by checking for a new firmware version and downloading it.
- `dht_sensor.h`: Contains functions to initialize the DHT11 sensor and read temperature and humidity.
- `aws_post.h`: Sends sensor data to AWS IoT via HTTP POST requests.
- `aws_read.h`: Retrieves data from AWS IoT via HTTP GET requests.
- `pins.h`: Defines the pin numbers for the various components.
- `display.h`: Manages the I2C LCD display for showing sensor data and status information.

## Main Workflow
1. **Wi-Fi Connection**: If not connected, the captive portal is opened to allow the user to enter Wi-Fi credentials.
2. **OTA Check**: The ESP32 checks for available firmware updates over the air.
3. **Sensor Reading**: Temperature, humidity, and analog sensor values are read.
4. **AWS Posting**: Sensor data is posted to AWS IoT using a secure HTTP request.
5. **AWS Data Retrieval**: Data is read from the AWS IoT shadow and printed to the serial monitor.
6. **Deep Sleep**: After each cycle, the ESP32 enters deep sleep mode for 5 minutes before waking up to repeat the process.

## Hardware Setup
- Connect the DHT11 sensor's data pin to **pin 14** on the ESP32.
- Connect the I2C LCD Display:
  - **SCL** to **pin 22**.
  - **SDA** to **pin 21**.
- Connect the analog sensor to **pin 34**.

## Installation
1. Clone this repository to your local machine.
2. Install the required libraries in the Arduino IDE (listed above).
3. Open the `.ino` file in the Arduino IDE and select the correct board (ESP32).
4. Upload the code to your ESP32.
5. Open the serial monitor to monitor the sensor data, AWS posting status, and OTA updates.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
