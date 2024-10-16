
# ESP32 + MQTT + DHT22 IoT Project

This project demonstrates how to use an ESP32 microcontroller to read temperature and humidity data from a DHT22 sensor and publish the readings to an MQTT broker. It connects to a WiFi network, reads the sensor data, and sends it to predefined MQTT topics. 

## Features
- Connect ESP32 to WiFi.
- Publish temperature and humidity data to an MQTT broker.
- Uses the DHT22 sensor for accurate temperature and humidity readings.
- Reconnects automatically if the MQTT connection is lost.

## Hardware Required
- ESP32 Development Board
- DHT22 Temperature & Humidity Sensor
- Jumper wires
- Breadboard (optional)

## Software Requirements
- Arduino IDE with ESP32 board support
- MQTT broker (e.g., [Eclipse MQTT](https://iot.eclipse.org/getting-started), [broker.mqttdashboard.com](https://www.mqtt-dashboard.com/))

## Libraries Used
1. [WiFi](https://www.arduino.cc/en/Reference/WiFi) - For managing WiFi connection on ESP32.
2. [PubSubClient](https://github.com/knolleary/pubsubclient) - For MQTT communication.
3. [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library) - For reading DHT22 sensor values.

## Pin Connections

| ESP32 Pin | DHT22 Pin |
|-----------|-----------|
| GPIO 5    | Data      |
| 3.3V      | VCC       |
| GND       | GND       |

Make sure to connect the **Data** pin of the DHT22 sensor to **GPIO5** on the ESP32 (you can change this pin in the code).

## Configuration

You will need to configure your WiFi credentials and MQTT settings in the code.

1. **WiFi Credentials**: 
   Replace the placeholders with your WiFi SSID and password:
   ```cpp
   const char* wifiSSID = "Your_SSID";
   const char* wifiPassword = "Your_PASSWORD";
   ```

2. **MQTT Broker Configuration**:
   Replace the placeholders with the MQTT broker URL, client ID, username, and password:
   ```cpp
   const char* mqttServer = "broker.mqttdashboard.com"; // Your MQTT broker
   const char* mqttUsername = "your_mqtt_username";
   const char* mqttPassword = "your_mqtt_password";
   const char* mqttClientId = "unique_client_id";
   ```

3. **MQTT Topics**:
   Modify the MQTT topics to suit your needs:
   ```cpp
   const char* temperatureTopic = "home/room/temperature";
   const char* humidityTopic = "home/room/humidity";
   ```

## How to Use

1. **Set up Arduino IDE**:
   - Install the necessary libraries (WiFi, PubSubClient, DHT Sensor Library).
   - Install ESP32 board support for Arduino IDE. Follow the instructions [here](https://github.com/espressif/arduino-esp32).

2. **Upload the Code**:
   - Open the provided sketch in Arduino IDE.
   - Select your ESP32 board from the "Tools" menu.
   - Connect the ESP32 to your computer via USB.
   - Click on **Upload**.

3. **Monitor the Data**:
   - Open the Serial Monitor (set baud rate to `115200`) to see the connection status and data being published to the MQTT topics.
   - You can subscribe to the `temperature` and `humidity` topics from any MQTT client to receive the sensor readings.

## Example Output

```
Connecting to Your_SSID
WiFi connected
IP address: 192.168.1.100
Attempting MQTT connection... connected
Temperature: 23.5 *C
Humidity: 60.2 %
Temperature: 23.4 *C
Humidity: 60.1 %
...
```

## Troubleshooting

1. **WiFi Connection Issues**:
   - Make sure your SSID and password are correct.
   - Check the signal strength of the WiFi network.
   - Verify that the ESP32 is within range of your WiFi router.

2. **MQTT Connection Issues**:
   - Ensure that your MQTT broker is up and running.
   - Check your broker URL, port, and credentials.
   - Verify that the client ID is unique on the network.

3. **Sensor Reading Issues**:
   - Check the wiring between the ESP32 and the DHT22 sensor.
   - Make sure you're using the correct DHT type in the code (`DHT22`).

## License

This project is open-source and licensed under the MIT License.

---


