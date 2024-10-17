
# ESP32 CO2 and Temperature Data Logger

This project demonstrates how to create a data logger using the ESP32, SD card, and a DS18B20 temperature sensor. The device logs temperature data along with CO2 sensor readings to an SD card at set intervals and enters deep sleep to conserve power. It also supports Wi-Fi connectivity for potential future expansions such as cloud-based logging.

## Project Details

The device captures and stores the following data:
- CO2 sensor readings (up to three sensors)
- Temperature readings from a DS18B20 sensor
- Humidity (if integrated with additional sensors)
- Timestamps using the RTC module (DS3231)

The data is saved to a CSV file on an SD card. The ESP32 enters deep sleep after logging to reduce power consumption.

## Hardware Required

- ESP32 development board
- DS18B20 temperature sensor
- SD card module (connected to SPI pins)
- DS3231 RTC module for real-time clock functionality
- Optional CO2 sensors (can connect up to 3)
- Wires and breadboard

## Wiring Diagram

| Component         | ESP32 Pin |
|-------------------|-----------|
| DS18B20 (Data)    | GPIO 21   |
| SD Card (CS)      | GPIO 5    |
| SD Card (MOSI)    | GPIO 23   |
| SD Card (MISO)    | GPIO 19   |
| SD Card (CLK)     | GPIO 18   |
| DS3231 (SDA)      | GPIO 21   |
| DS3231 (SCL)      | GPIO 22   |

## Software Requirements

- Arduino IDE with ESP32 support
- Required Libraries:
  - **RTClib**: For working with the DS3231 RTC module
  - **OneWire**: For interfacing with the DS18B20 sensor
  - **DallasTemperature**: To read temperature from the DS18B20 sensor
  - **WiFi**: To enable Wi-Fi functionality on the ESP32
  - **FS** and **SD**: For reading and writing to the SD card

You can install the libraries through the Arduino Library Manager.

## Code Explanation

### Main Functions

1. **`setup()`**:
   - Connects to Wi-Fi.
   - Initializes the SD card and creates a log file.
   - Initializes the RTC and prepares for deep sleep.
   
2. **`getReadings()`**:
   - Reads the temperature from the DS18B20 sensor.
   
3. **`logSDCard()`**:
   - Logs sensor data (temperature, CO2, humidity) to the SD card.

4. **`writeFile()` and `appendFile()`**:
   - Write or append data to the SD card.

5. **Deep Sleep**:
   - After logging data, the ESP32 enters deep sleep for 10 minutes to save power.

### File Format

Data is logged to a CSV file on the SD card. The log file format is:

```
MilliSec,Stamp,Date,Time,CO2_1,CO2_2,CO2_3,Temperature_1(C),Humidity_1,Temperature_2(C),Humidity_2,Temperature_3(C),Humidity_3
```

Where:
- `CO2_1`, `CO2_2`, and `CO2_3` are the readings from up to 3 CO2 sensors.
- `Temperature_1(C)` is the temperature reading from the DS18B20 sensor.

### Customization

- **Logging Interval**: The logging interval is set to 20 seconds (`LOG_INTERVAL`). You can adjust this based on your requirements.
- **Sleep Duration**: The deep sleep time is set to 10 minutes (`TIME_TO_SLEEP` = 600 seconds). You can change this for longer or shorter sleep cycles.

## How to Use

1. **Install the required libraries** in your Arduino IDE.
2. **Upload the code** to your ESP32 board.
3. **Connect the sensors** as described in the wiring diagram.
4. **Insert an SD card** into the SD card module.
5. **Power the ESP32**, and it will start logging data to the SD card.

## Future Enhancements

- Add more sensors (humidity, additional temperature sensors, etc.).
- Implement Wi-Fi data uploading to a server or cloud platform.
- Add real-time monitoring via a web interface.

## License

This project is based on the work of Rui Santos and is provided for educational purposes. See the original project details at [Random Nerd Tutorials](https://randomnerdtutorials.com).
```

### Explanation of Sections:
- **Project Overview**: Describes what the project does.
- **Hardware and Software Requirements**: Lists components and software.
- **Wiring Diagram**: Provides pin connections.
- **Code Explanation**: Gives details about the functions in the code.
- **How to Use**: Instructions for setting up and running the project.
- **Future Enhancements**: Ideas for expanding the project.


