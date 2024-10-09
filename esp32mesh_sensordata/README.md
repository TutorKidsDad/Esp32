# ESP32/ESP8266 Mesh Network with BME280 Sensors

This project demonstrates how to create a mesh network using ESP32/ESP8266 devices that read temperature, humidity, and pressure data from BME280 sensors. The data is then visualized on a web page, allowing users to monitor environmental conditions in real-time.

## Features

- Mesh Networking: Multiple ESP32/ESP8266 devices can communicate with each other.
- BME280 Sensors: Collects temperature, humidity, and pressure readings.
- Web Server: A simple web interface to visualize sensor data in real-time.

## Components Required

- ESP32 or ESP8266 board
- BME280 sensor module
- Jumper wires
- Breadboard (optional)

## Libraries Required

- Adafruit BME280 Library
- PainlessMesh
- Arduino_JSON
- ESPAsyncWebServer

## Installation

1. **Clone the Repository**:
   git clone https://github.com/yourusername/your-repo.git
   cd your-repo

2. **Install Required Libraries**:
   Use the Arduino Library Manager to install the required libraries mentioned above.

3. **Upload the Sketch**:
   Open the .ino file in the Arduino IDE, replace Your_SSID and Your_PASSWORD in the code with your Wi-Fi credentials, and upload the sketch to your ESP32/ESP8266.

4. **Upload the HTML File**:
   Create an index.html file and upload it to the SPIFFS filesystem of your ESP32/ESP8266. Use the "ESP32 Sketch Data Upload" tool available in the Arduino IDE for this purpose.

## Usage

1. **Connect Power**: Power on your ESP32/ESP8266 board.
2. **Open Serial Monitor**: Check the IP address assigned to your device once it's connected to Wi-Fi.
3. **Access the Web Interface**: Open a web browser and enter the IP address. You should see the sensor data being displayed on the web page.

## Data Visualization

The web interface will display the following sensor data:

- Temperature: Current temperature in °C
- Humidity: Current humidity percentage
- Pressure: Current pressure in hPa

The data is fetched every 5 seconds to provide real-time updates.

## Code Explanation

- Mesh Network: The project uses the painlessMesh library to create a mesh network where ESP32/ESP8266 devices communicate wirelessly.
- Sensor Data Collection: The BME280 sensor readings are collected and formatted as JSON.
- Web Server: An asynchronous web server serves the HTML page and provides a JSON endpoint for sensor data.

## Troubleshooting

- Ensure all wiring is correct between the ESP32/ESP8266 and the BME280 sensor.
- Check your Wi-Fi credentials and network connection.
- Monitor the Serial Output for any error messages during initialization.

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Acknowledgements

- Rui Santos for the original inspiration from Random Nerd Tutorials.
- Adafruit for the BME280 library and resources.

