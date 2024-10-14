This Arduino sketch is designed for an ESP32 (or ESP8266) to interface with GPS and accelerometer sensors, collect data, and send it to AWS IoT Core using MQTT. Below is a breakdown of its components:

### Libraries Included:
- **TinyGPSPlus**: Parses GPS data.
- **SoftwareSerial**: Creates a serial communication link with the GPS device.
- **WiFi (ESP32/ESP8266)**: Manages Wi-Fi connections.
- **WiFiClientSecure**: Manages secure connections over Wi-Fi.
- **MQTT**: Handles MQTT protocol for publishing/subscribing to topics.
- **ArduinoJson**: Constructs JSON objects for data transmission.
- **Adafruit_Sensor & Adafruit_ADXL345_Unified**: Communicates with an ADXL345 accelerometer.

### Constants and Variables:
- `RXPin` and `TXPin` define the GPS module's communication pins.
- `GPSBaud`: Baud rate for GPS communication.
- **AWS IoT Topics**: `MQTT_SUB_TOPIC` and `MQTT_PUB_TOPIC` handle communication with AWS IoT.
- Variables like `xAccl`, `yAccl`, `zAccl`, `latitude`, `longitude`, and `aspeed` store accelerometer and GPS data.

### Main Functions:

1. **NTPConnect**: Sets the device time using the NTP (Network Time Protocol) servers.
2. **messageReceived**: Handles MQTT messages received from AWS IoT Core (not used extensively in this sketch).
3. **connectToMqtt**: Connects to AWS IoT Core via MQTT, subscribing to a topic for device updates.
4. **connectToWiFi**: Establishes a Wi-Fi connection using predefined credentials (`ssid` and `pass` in `secrets.h`).
5. **sendData**: Sends sensor data to AWS IoT Core in JSON format, publishing the data to a specific MQTT topic.
6. **displayInfo**: Reads and displays the GPS data (latitude, longitude, speed) and outputs them to the serial monitor.
7. **smartDelay**: Delays execution but continues to feed the GPS object by reading incoming data.
8. **loop**: This is the core execution loop:
   - It checks if the MQTT client is connected. If not, it reconnects.
   - It reads accelerometer data (x, y, z axes).
   - It calls `displayInfo` to display the GPS and sensor data.
   - Every second, it sends the collected data to AWS IoT Core using `sendData`.

### Summary:
This program reads GPS and accelerometer data, formats it into a JSON object, and publishes it to AWS IoT Core. It handles Wi-Fi reconnection, GPS data parsing, accelerometer readings, and secure communication using MQTT. The collected data includes the device's latitude, longitude, speed, and acceleration, which are sent to AWS periodically.
