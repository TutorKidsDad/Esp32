# ESP32 Project with OTA, Captive Portal, DHT11 Sensor, LCD Display, and ThingSpeak Integration

This project showcases an ESP32 implementation with the following features:
- **WiFi Captive Portal**: Allows easy connection to WiFi via a portal.
- **Over-the-Air (OTA) Updates**: Automatically checks for firmware updates and performs OTA updates when a newer version is available.
- **DHT11 Sensor**: Reads temperature and humidity data.
- **Analog Input**: Reads analog values from pin 34.
- **LCD Display**: Displays sensor data and system messages on a 20x4 I2C LCD.
- **ThingSpeak Integration**: Posts sensor data to a ThingSpeak channel and reads data from another channel.
- **Deep Sleep**: Puts the ESP32 into deep sleep mode when required.

## Project Structure

- **`dht_sensor.h`**: Initializes and reads data from the DHT11 sensor.
- **`display.h`**: Initializes and updates the I2C LCD screen.
- **`ota_update.h`**: Handles OTA updates and captive portal for WiFi connection.
- **`thingspeak_post.h`**: Posts sensor data to ThingSpeak.
- **`thingspeak_read.h`**: Reads data from a ThingSpeak channel.
- **`pins.h`**: Defines pin assignments for the DHT11 sensor, LCD, and analog input.
  
## Features and Code Explanation

### 1. DHT11 Sensor (`dht_sensor.h`)
- The DHT11 sensor is used to measure temperature and humidity.
- **Functions**:
  - `initDHT()`: Initializes the DHT11 sensor.
  - `readTemperature()`: Reads the temperature in Celsius.
  - `readHumidity()`: Reads the humidity as a percentage.
  
### 2. LCD Display (`display.h`)
- A 20x4 I2C LCD is used to display sensor readings and system status.
- **Functions**:
  - `init_display()`: Initializes the LCD screen, displays the device name, and checks WiFi status.
  - `update_screen()`: Updates the screen with the latest sensor readings and system messages.

### 3. OTA Update (`ota_update.h`)
- The ESP32 automatically checks for firmware updates from a server and performs an OTA update if a newer version is available.
- **Functions**:
  - `connectToWiFi()`: Connects to WiFi via a captive portal, making it easy to configure WiFi credentials.
  - `checkForUpdate()`: Checks for a new firmware version and initiates an update if necessary.
  - `otaUpdate()`: Performs the actual OTA update.
  - `goToDeepSleep()`: Puts the ESP32 into deep sleep mode for power saving.

### 4. ThingSpeak Data Posting and Reading
- **Posting Data (`thingspeak_post.h`)**: 
  - Posts temperature, humidity, and analog values to a ThingSpeak channel.
  - **Function**: `postDataToThingSpeak()`
  
- **Reading Data (`thingspeak_read.h`)**: 
  - Reads data from another ThingSpeak channel.
  - **Function**: `readDataFromThingSpeak()`
  
### 5. Pin Configuration (`pins.h`)
- **Pin Assignments**:
  - DHT11 Sensor: GPIO 14 (DHTPIN)
  - Analog Input: GPIO 34
  - LCD (I2C): SDA (GPIO 21), SCL (GPIO 22)

## Wiring

- **DHT11 Sensor**:
  - Data Pin: GPIO 14
  - Power and Ground: 3.3V and GND on ESP32
- **Analog Sensor**: Connected to GPIO 34
- **LCD**: 
  - I2C SDA: GPIO 21
  - I2C SCL: GPIO 22

## Required Libraries

Make sure to install the following libraries in the Arduino IDE:
- [WiFiManager](https://github.com/tzapu/WiFiManager)
- [DHT Sensor Library](https://github.com/adafruit/DHT-sensor-library)
- [ThingSpeak](https://github.com/mathworks/thingspeak-arduino)
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [LiquidCrystal_I2C](https://github.com/johnrickman/LiquidCrystal_I2C)

## Setup Instructions

1. Clone or download this repository.
2. Open the project in Arduino IDE.
3. Modify `thingspeak_post.h` and `thingspeak_read.h` to add your ThingSpeak API keys and Channel IDs.
4. Flash the ESP32 with the code.
5. Power up the ESP32, and connect to the captive portal to configure WiFi.
6. The device will:
   - Check for OTA updates.
   - Read temperature and humidity from the DHT11 sensor.
   - Post the sensor readings to ThingSpeak.
   - Read data from another ThingSpeak channel.
   - Display the data on the LCD.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

