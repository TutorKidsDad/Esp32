# ESP32/ESP8266 Moisture Sensor with WiFi & ThingSpeak

This project demonstrates the use of ESP32 or ESP8266 to read data from moisture sensors, filter the readings, and publish the results to a ThingSpeak channel. The device also supports deep sleep to save power and wakes up periodically to gather and send data.

## Features

- Measure moisture from two sensors - Gypsum/watermark sensor.
- Publish moisture data to ThingSpeak using WiFi.
- AutoConnect library for WiFi connection management.
- Deep sleep mode to save power.
- Median filter for stable sensor readings.
- WiFi multi-mode connection for fallback in case of primary network failure.
- Real-time WiFi status indication using LEDs.

## Hardware Requirements

- **ESP32 or ESP8266** microcontroller
- **Two Moisture Sensors**
- **4700Ω Resistor** (for each sensor)
- **WiFi connection**
- **LEDs** for WiFi status indication (optional)

## Software Requirements

- Arduino IDE with ESP32 or ESP8266 boards installed.
- ThingSpeak account.
- AutoConnect library for WiFi management.
- PubSubClient library for MQTT.
- WiFiMulti library for multiple WiFi networks.

## ThingSpeak Setup

1. Sign up for a [ThingSpeak account](https://thingspeak.com).
2. Create a new channel to receive the moisture data.
3. Take note of your channel number and write API key, as these will be used in the code.

## Circuit Diagram

- Connect each moisture sensor to the ESP32 or ESP8266.
- Include a 4700Ω resistor in the sensor circuit.
- Connect LEDs to GPIO pins to show WiFi status.

## Pin Configuration

The code dynamically switches the sensor reading pins between two sets to minimize interference:

- Sensor 1: Digital Pin 18, Analog Pins 34 and 35.
- Sensor 2: Digital Pin 16, Analog Pins 32 and 33.

LED indicators are connected as follows:

- WiFi Status LED: Pin 15
- Additional Status LED: Pin 2

## AutoConnect Customization

To change the AutoConnect portal credentials (AP name and password), modify the following files:

- **AutoConnectDefs.h**: Update credentials.
- **AutoConnectLabels.h**: Change labels on the AutoConnect frontend page.

## How to Flash the Code

1. Download and install [esptool](https://github.com/espressif/esptool).
2. Use the following command to erase the existing flash:
    ```bash
    esptool.py erase_flash
    ```
3. Upload the code via Arduino IDE or command line.

## WiFi Configuration

The code supports multiple WiFi networks using `WiFiMulti`. Add more networks in the following section:

wifiMulti.addAP("SSID1", "PASSWORD1");
wifiMulti.addAP("SSID2", "PASSWORD2");


## Soil Moisture Sensor with ESP32/ESP8266 and ThingSpeak

This code reads data from two soil moisture sensors and posts the results to ThingSpeak. It uses ESP32/ESP8266, with WiFi connection, sensor data filtering, and deep sleep for power saving.
Code Sections
1. Libraries and Definitions

    Includes WiFi libraries for ESP32/ESP8266 and defines macros for chip ID and API integration.

2. WiFi and ThingSpeak Setup

    Configures WiFi connection using WiFiMulti for multiple networks.
    Sets ThingSpeak channel number and API key.

3. Timer Configuration

    timerDelay sets the delay between data posts (10 seconds in this case).

4. Data Structure for Moisture Values

    Stores sensor data (moisture and resistance) for two sensors.

5. Sensor Pin Definitions

    Defines sensor pins for reading voltage and controlling power.

6. Deep Sleep Configuration

    Sets ESP deep sleep duration for power saving.

7. WiFi Connection Setup

    Initializes WiFi in station mode and attempts to connect to saved networks.

8. LED Pin Setup

    Configures LEDs to indicate WiFi and system status.

9. Power Control for Sensors

    Alternates between sensor pins to control power and read values.

10. Moisture Data Sorting

    Sorts sensor data to remove noise and get stable readings.

11. Main Setup

    Initializes WiFi, ThingSpeak, sensor pins, and enables deep sleep.

12. Sensor Data Processing

    Reads sensor data, calculates moisture, filters values, and posts to ThingSpeak if WiFi is connected.


