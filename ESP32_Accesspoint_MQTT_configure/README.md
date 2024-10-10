# README: ESP32/ESP8266 Wi-Fi Temperature & Humidity Logger

This project demonstrates how to use the ESP32/ESP8266 to create a Wi-Fi-enabled temperature and humidity logger with MQTT server configuration. It uses the DHT11 sensor for temperature and humidity measurements and allows dynamic Wi-Fi and MQTT configuration through a web portal.

## Features
- Reads temperature and humidity data using the DHT11 sensor.
- Uses `WiFiManager` to configure Wi-Fi credentials dynamically.
- Configures MQTT server and port via a web portal.
- Stores configuration data in LittleFS (replaces SPIFFS).
- Logs data to the MQTT server.
- Supports static IP configuration.

## Components
- ESP32/ESP8266 microcontroller
- DHT11 temperature and humidity sensor
- Wi-Fi connection
- LittleFS for file system handling
- WiFiManager for easy configuration

## Libraries Used
- [WiFi](https://github.com/esp8266/Arduino)
- [WiFiManager](https://github.com/tzapu/WiFiManager) for handling Wi-Fi credentials
- [LittleFS](https://github.com/earlephilhower/LittleFS) for file storage
- [DHT](https://github.com/adafruit/DHT-sensor-library) for reading DHT11 sensor data
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson) for JSON handling

## Setup Instructions

1. **Hardware Connection**:
    - Connect the DHT11 sensor to GPIO pin 0 of the ESP32/ESP8266.

2. **Code Setup**:
    - Install the required libraries:
      - `WiFiManager`
      - `LittleFS`
      - `ArduinoJson`
      - `DHT sensor library`
    - Copy the provided code into your Arduino IDE.
    - Adjust the DHT sensor pin (`DHTPIN`) if necessary.

3. **MQTT Server Configuration**:
    - Open the `WiFiManager` web portal by connecting to the access point `AutoConnectAP` (default password: `password`).
    - Set up your Wi-Fi credentials.
    - Input your MQTT server and port through the portal.

4. **Data Logging**:
    - The device reads temperature and humidity data every 10 seconds and sends it to the configured MQTT server.
The data (temperature and humidity readings from the DHT11 sensor) is being sent to the **MQTT server** that you configure through the `WiFiManager` web portal. Specifically, the readings are sent as an HTTP GET request to the MQTT server using the following format:

```
GET <postStr> HTTP/1.1
Host: <mqtt_server>
Connection: close
```

- `mqtt_server`: This is the address of your MQTT server (configured in the web portal).
- `postStr`: This contains the data, including the API key and the sensor readings (`field1` for temperature, `field2` for humidity).
  
For example, if the MQTT server is set to `api.thingspeak.com`, the data will be sent to that server. The `postStr` would look something like this:

```
YOUR_API_KEY&field1=<temperature>&field2=<humidity>
```

The actual destination is defined by the `mqtt_server` you configure, and the data can be sent to a service like ThingSpeak, or any MQTT broker that accepts HTTP GET requests.

## Code Walkthrough

### 1. **File System Setup**:
The code initializes `LittleFS` to store and load configuration data such as MQTT server, port, and API key. It reads the configuration from `/config.json`.

```cpp
if (LittleFS.begin()) {
    // Load configuration file
    if (LittleFS.exists("/config.json")) {
        // Parse JSON config
    }
}
```

### 2. **WiFiManager Configuration**:
`WiFiManager` provides an easy way to connect the ESP device to a Wi-Fi network by setting up a configuration portal when no network is configured or when the user needs to change the Wi-Fi or MQTT settings.

```cpp
WiFiManager wifiManager;
wifiManager.setSaveConfigCallback(saveConfigCallback);
wifiManager.autoConnect("AutoConnectAP", "password");
```

### 3. **DHT Sensor Reading**:
The DHT11 sensor is read in the loop, and its temperature and humidity values are sent to the MQTT server.

```cpp
humi = dht.readHumidity();
temp = dht.readTemperature();
```

### 4. **MQTT Communication**:
After reading the sensor data, it forms an HTTP request to send the data to the MQTT server.

```cpp
if (client.connect(mqtt_server, 1883)) {
    String postStr = apiKey + "&field1=" + String(temp) + "&field2=" + String(humi);
    client.print("GET " + postStr + " HTTP/1.1\r\nHost: " + mqtt_server + "\r\nConnection: close\r\n\r\n");
}
```

### 5. **Configuration Save**:
If any configuration changes are made via the web portal, the new settings are saved in `config.json` in LittleFS.

```cpp
if (shouldSaveConfig) {
    File configFile = LittleFS.open("/config.json", "w");
    json.printTo(configFile);
}
```

## Dependencies
- ESP8266 or ESP32 board
- DHT sensor library
- WiFiManager library
- ArduinoJson library
- LittleFS library

## Notes
- The device reconnects to the saved Wi-Fi and MQTT server automatically after a power cycle.
- You can reset the device’s settings by uncommenting `wifiManager.resetSettings();` in the setup function.
- If static IP is required, adjust `static_ip`, `static_gw`, and `static_sn` as needed.

## Future Enhancements
- Add error handling for sensor readings.
- Implement deep sleep to save power between readings.
- Support for additional sensors or MQTT topics.

---

Feel free to customize and extend this code to suit your specific use case!
