# IoT-Based Soil Moisture and Temperature Monitoring System with ESP32/ESP8266

This project is an IoT-based system for monitoring soil moisture and temperature using an ESP32 (or ESP8266) microcontroller, a DS18B20 temperature sensor, and a soil moisture sensor. Data is transmitted to ThingSpeak for real-time monitoring over the internet.

## Features

- **Soil Moisture Sensor**: Measures the soil moisture content.
- **DS18B20 Temperature Sensor**: Monitors the temperature of the soil.
- **WiFi Connectivity**: Sends the data to the cloud (ThingSpeak) over WiFi.
- **Non-blocking Code**: Uses `millis()` for time intervals, allowing the program to run continuously without being delayed.

## Hardware Requirements

- ESP32 (or ESP8266) Microcontroller
- DS18B20 Temperature Sensor
- Soil Moisture Sensor
- 4.7kΩ Resistor (for DS18B20)
- Jumper Wires
- Breadboard

## Circuit Diagram

- **DS18B20**:
  - Data Pin connected to GPIO 4 on ESP32 (can be changed).
  - 4.7kΩ pull-up resistor between data pin and 3.3V.
- **Soil Moisture Sensor**:
  - Analog output connected to A0 pin of the ESP32.

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software)
- ESP32 or ESP8266 Board Support (can be added via Arduino Board Manager)
- [OneWire Library](https://github.com/PaulStoffregen/OneWire)
- [DallasTemperature Library](https://github.com/milesburton/Arduino-Temperature-Control-Library)
- WiFi and WiFiClient for ESP32 or ESP8266 (comes with Arduino core for these boards)

## Installation Steps

1. **Install Libraries**:
   Open the Arduino IDE and install the following libraries:
   - OneWire
   - DallasTemperature

2. **Set Up ThingSpeak Account**:
   - Go to [ThingSpeak](https://thingspeak.com/), create an account, and create a new channel.
   - Note the **Write API Key** from the ThingSpeak channel as it will be used in the code.

3. **Wiring**:
   - Follow the circuit diagram provided to connect the DS18B20 and soil moisture sensor to your ESP32/ESP8266.

4. **Upload Code**:
   - Open the provided `.ino` code in the Arduino IDE.
   - Replace the WiFi credentials and ThingSpeak API key in the code.
   - Choose the correct board (ESP32 or ESP8266) and upload the code.

5. **Monitor Data**:
   - Once the ESP32 connects to WiFi, it will start sending temperature and soil moisture data to ThingSpeak at a 15-second interval.
   - You can view the data on your ThingSpeak channel.

## Code Overview

The code reads the temperature from the DS18B20 sensor and the moisture level from the soil moisture sensor. It uses the `millis()` function for timing, so no `delay()` function is used. This allows other tasks to be executed simultaneously.

- **Temperature**: Measured in degrees Celsius and sent to ThingSpeak.
- **Soil Moisture**: Measured as a percentage of moisture and sent to ThingSpeak.

## Usage

1. Power up the ESP32/ESP8266.
2. The ESP will connect to your specified WiFi network.
3. Every 15 seconds, it reads the soil moisture and temperature and sends the data to ThingSpeak.

## ThingSpeak Fields

- **Field 7**: Soil Moisture (in percentage).
- **Field 8**: Temperature (in Celsius).

## Future Improvements

- Add more sensors for better environmental monitoring.
- Implement battery power management to use this system in a remote location.
- Send alerts based on soil moisture thresholds to avoid overwatering or underwatering.

## License

This project is open-source and licensed under the MIT License. Feel free to modify and use it for personal or educational purposes.

