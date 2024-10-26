
# ESP32 DHT Sensor with mDNS and Web Server

This project utilizes an ESP32 microcontroller to read temperature and humidity data from a DHT11 sensor and display the data on an OLED screen. Additionally, it sets up a web server with mDNS support to access the sensor readings from a mobile device or computer.

## Features

- Reads temperature and humidity data using a DHT11 sensor.
- Displays readings on a 128x128 OLED screen.
- Sets up a web server to serve sensor data.
- Uses mDNS to provide easy access to the web server via a hostname.

## Hardware Requirements

- ESP32 Development Board
- DHT11 Temperature and Humidity Sensor
- OLED Display (128x128 pixels, SSD1351)
- Jumper wires

## Connections

- **DHT11 Sensor:**
  - VCC to 3.3V
  - GND to GND
  - Data pin to GPIO 19 (DHTPIN)

- **OLED Display:**
  - VCC to 3.3V
  - GND to GND
  - SCL to GPIO 18 (SCLK_PIN)
  - SDA to GPIO 23 (MOSI_PIN)

## Software Requirements

- Arduino IDE
- ESP32 Board package
- Required Libraries:
  - Adafruit GFX Library
  - Adafruit SSD1351 Library
  - DHT Sensor Library
  - JPEGDecoder Library
  - WiFiManager Library
  - ESPmDNS Library

## Installation

1. **Clone the Repository:**
   ```bash
   git clone <repository-url>
   cd <repository-directory>
   ```

2. **Install Libraries:**
   Open the Arduino IDE, go to **Sketch > Include Library > Manage Libraries**, and search for the required libraries to install them.

3. **Configure Wi-Fi:**
   - The ESP32 will create a captive portal named `DHT_Sensor` if it can't connect to a known Wi-Fi network. Connect to this network to enter your Wi-Fi credentials.

4. **Upload the Code:**
   Open the `.ino` file in the Arduino IDE, select your ESP32 board and port, and upload the code.

## Usage

1. **Connect to the Captive Portal:**
   If the ESP32 cannot find a saved Wi-Fi network, it will create an access point named `DHT_Sensor`. Connect to this network using your mobile device.

2. **Enter Wi-Fi Credentials:**
   Open a browser and go to `http://192.168.4.1` to enter your Wi-Fi credentials.

3. **Access the Sensor Data:**
   Once connected to your Wi-Fi, you can access the sensor readings by navigating to `http://dhtsensor.local` in a web browser.

## Troubleshooting

- If you cannot access the web server, ensure that your mobile device is connected to the same Wi-Fi network as the ESP32.
- Make sure the DHT11 sensor is connected properly and that the code is uploaded successfully.

## License

This project is open-source and available under the MIT License.


