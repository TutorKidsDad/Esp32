# ESP32 Data Logging to Firebase Realtime Database

## License
This project is based on the example provided by Rui Santos from [RandomNerdTutorials](https://randomnerdtutorials.com/esp32-data-logging-firebase-realtime-database/). The code is open-source and free to use as described in the provided license.

## Project Overview
This project demonstrates how to use an ESP32 to log sensor data from a BME280 sensor (temperature, humidity, and pressure) to Firebase Realtime Database. The ESP32 connects to a Wi-Fi network, collects data from the sensor, and sends it to Firebase with a timestamp retrieved from an NTP (Network Time Protocol) server.

## Components Used
- **ESP32**: Main microcontroller that connects to Wi-Fi and Firebase.
- **BME280 Sensor**: Used to measure temperature, humidity, and pressure.
- **Firebase Realtime Database**: Cloud database for storing sensor data.
- **NTP Server**: Used to get the current timestamp for each reading.

## Libraries Required
- `WiFi.h`: For ESP32 to connect to the Wi-Fi network.
- `Firebase_ESP_Client.h`: For interacting with Firebase Realtime Database.
- `Wire.h`, `Adafruit_Sensor.h`, `Adafruit_BME280.h`: For reading sensor data from the BME280 sensor.
- `time.h`: For retrieving the current time from an NTP server.

## Key Features
- **Wi-Fi Connection**: The ESP32 connects to the specified Wi-Fi network using the SSID and password defined in the code.
- **BME280 Sensor Initialization**: The code initializes the BME280 sensor over I2C. If the sensor is not detected, an error message is printed.
- **Firebase Initialization**: 
  - The ESP32 connects to Firebase using API key, user email, password, and the database URL.
  - A unique User ID (UID) is retrieved after signing in to Firebase.
  - The `Firebase.reconnectWiFi(true)` ensures automatic reconnection to Wi-Fi if it is lost.
- **Data Logging**: 
  - The ESP32 reads temperature, humidity, and pressure from the BME280 sensor.
  - A timestamp is obtained using the NTP server (`pool.ntp.org`).
  - Data is structured into JSON format and sent to Firebase under a specific path based on the UID and timestamp.
  - The data includes:
    - `temperature`: Sensor's temperature reading.
    - `humidity`: Sensor's humidity reading.
    - `pressure`: Sensor's pressure reading.
    - `timestamp`: Current time when data is logged.

## How it Works
1. **Setup**:
   - The ESP32 connects to Wi-Fi.
   - Firebase authentication is initialized using the user's credentials.
   - The BME280 sensor is initialized for I2C communication.
   - The current time is fetched from an NTP server.

2. **Data Logging**:
   - Every 3 minutes, the ESP32 fetches temperature, humidity, and pressure readings.
   - It also retrieves the current timestamp from the NTP server.
   - The data is structured as a JSON object and sent to Firebase under the path `/UsersData/{UID}/readings/{timestamp}`.

## Timer Control
- A non-blocking timer is implemented using `millis()`. New data is logged every 3 minutes (`timerDelay = 180000` milliseconds).
- This prevents the `loop()` function from being blocked, allowing the ESP32 to perform other tasks if necessary.

## Code Structure
- **`initBME()`**: Initializes the BME280 sensor. If the sensor is not found, it prints an error message and halts the program.
- **`initWiFi()`**: Connects the ESP32 to the Wi-Fi network and prints the local IP address.
- **`getTime()`**: Retrieves the current epoch time from the NTP server.
- **`loop()`**: 
  - Checks if Firebase is ready and if the timer interval has elapsed.
  - Fetches sensor readings and timestamp, packages them into JSON, and uploads them to Firebase.

## Firebase Database Structure
- The data is logged in the following structure:
- /UsersData/{UID}/readings/{timestamp}/

   - temperature: "xx.x"
   - humidity: "xx.x"
   - pressure: "xx.x"
   - timestamp: "xxxxxxx"


