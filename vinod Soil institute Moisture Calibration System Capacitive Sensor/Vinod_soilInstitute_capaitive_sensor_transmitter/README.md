Here's the `README.md` file for your project:

---

# ESP32 ThingSpeak Data Logger with DHT11, DS18B20, Soil Moisture Sensor, and 16x2 I2C LCD Display

This project uses an ESP32 to read data from a DHT11 temperature and humidity sensor, a DS18B20 temperature sensor, and a soil moisture sensor. The data is displayed on a 16x2 I2C LCD and sent to ThingSpeak for remote monitoring. The ESP32 is optimized for battery-powered applications with deep sleep mode to conserve energy.

## Features
- **WiFi Connectivity**: Connects to a WiFi network to send data to ThingSpeak.
- **DHT11 Sensor**: Measures temperature and humidity.
- **DS18B20 Sensor**: Measures precise temperature readings.
- **Soil Moisture Sensor**: Monitors the moisture level of soil.
- **I2C LCD Display**: Shows the soil moisture percentage, temperature (from both DHT11 and DS18B20), and humidity.
- **ThingSpeak Integration**: Sends data to ThingSpeak for remote monitoring and logging.
- **Power Conservation**: Uses deep sleep mode to reduce power consumption, waking up every 10 minutes to read sensors and send data.

## Components Used
- **ESP32**: Main microcontroller for handling sensor data, WiFi connectivity, and ThingSpeak communication.
- **DHT11 Sensor**: For temperature and humidity measurements.
- **DS18B20 Temperature Sensor**: For accurate temperature readings.
- **Soil Moisture Sensor**: For monitoring soil moisture levels.
- **16x2 I2C LCD**: For displaying sensor data locally.
- **ThingSpeak**: Platform for storing and visualizing data remotely.
- **Battery Power**: Optional for remote/battery-powered operation with deep sleep mode enabled.

## Circuit Diagram
- **DHT11 Sensor**:
  - Data pin connected to ESP32 pin 2.
- **DS18B20 Sensor**:
  - Data pin connected to ESP32 pin 4.
- **Soil Moisture Sensor**:
  - Connected to ESP32 analog pin A0.
- **16x2 I2C LCD**:
  - SDA connected to ESP32 pin 21.
  - SCL connected to ESP32 pin 22.

## How It Works
1. **Sensor Readings**: The ESP32 reads temperature and humidity from the DHT11, temperature from the DS18B20, and soil moisture from the analog sensor.
2. **Display**: The sensor values are displayed on a 16x2 I2C LCD for local monitoring.
3. **ThingSpeak Upload**: The data is sent to ThingSpeak over WiFi.
4. **Deep Sleep**: After completing the data reading and upload, the ESP32 enters deep sleep for 10 minutes to conserve battery power.

## Setup Instructions

### Hardware
1. Connect the DHT11, DS18B20, soil moisture sensor, and 16x2 I2C LCD to the ESP32 as per the circuit diagram.
2. Ensure that the soil moisture sensor is inserted into the soil.

### Software
1. Install the required libraries in your Arduino IDE:
   - **WiFi.h**
   - **ThingSpeak.h**
   - **Adafruit_Sensor.h**
   - **DHT.h**
   - **OneWire.h**
   - **DallasTemperature.h**
   - **LiquidCrystal_I2C.h**
2. Replace the following placeholders in the code:
   - `ssid`: Your WiFi SSID.
   - `password`: Your WiFi password.
   - `channelID`: Your ThingSpeak channel ID.
   - `apiKey`: Your ThingSpeak API key.
3. Upload the code to the ESP32 using the Arduino IDE.

### Code Summary
The ESP32 reads data from three sensors and displays the results on an LCD. It then sends this data to ThingSpeak and enters deep sleep mode for 10 minutes before repeating the process. 

#### Sensors
- **Soil Moisture**: Measures and maps the sensor values to a percentage.
- **DHT11**: Measures temperature and humidity.
- **DS18B20**: Measures temperature.

#### Display
The 16x2 I2C LCD displays:
- Soil Moisture (%)
- Temperature (from both sensors)
- Humidity

#### Power Conservation
Deep sleep mode is used to conserve battery power, and the ESP32 wakes up after 10 minutes to perform the readings again.

## Applications
- **Smart Irrigation Systems**: Monitor soil moisture and automate watering based on sensor data.
- **Environmental Monitoring**: Collect and log temperature and humidity data for agricultural or environmental purposes.
- **IoT Projects**: Send data to the cloud (ThingSpeak) for remote monitoring and analysis.

## Future Improvements
- Add more sensors for better environmental data logging.
- Implement user-defined thresholds to trigger alerts when conditions are met (e.g., low soil moisture).
- Add a real-time clock (RTC) for more accurate sleep intervals.

## License
This project is open-source under the MIT License.

