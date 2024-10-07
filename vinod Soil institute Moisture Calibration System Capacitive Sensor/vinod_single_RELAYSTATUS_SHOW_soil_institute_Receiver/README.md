
# IoT-Based Automated Pump Control System using ESP32 and ThingSpeak

This project is an IoT-based automated pump control system designed to monitor soil moisture and manage water pumps based on sensor readings. The system uses an ESP32 microcontroller to communicate with the ThingSpeak platform for data reading and writing. The system automatically controls a relay that turns a water pump on or off, depending on the soil moisture levels.

## Features
- Automated relay control based on soil moisture readings.
- Uses ThingSpeak to read and store data in real-time.
- WiFi connectivity for remote monitoring and control.
- Automatic pump control based on moisture thresholds.
- Configurable high and low moisture settings via ThingSpeak.
- ESP32-based system with digital control outputs.

## Components Used
- **ESP32**: The main microcontroller for WiFi and ThingSpeak connectivity.
- **Relay Module**: To control the water pump.
- **Water Pump**: Activated based on soil moisture levels.
- **WiFi**: For connectivity to the ThingSpeak cloud.
- **ThingSpeak**: Used to read sensor data and update control statuses.

## Prerequisites

Before you begin, ensure you have the following installed:
- Arduino IDE (or any other ESP32 compatible IDE).
- ESP32 board libraries.
- ThingSpeak library (`ThingSpeak.h`).
- WiFi credentials for your network.

## Hardware Setup

- Connect the ESP32 to the relay module. The relay module controls the water pump.
- The soil moisture sensor will provide readings that determine if the pump should be turned on or off.
- **Pin Assignments**:
  - Relay pin: GPIO 12 (`RELAY_PIN`)
  - Pump pin: GPIO 26 (`Pump`)
  - WiFi status pin: GPIO 27 (`Wifi_Status`)

## Software Configuration

1. Clone this repository or download the project files.
2. Open the project in your Arduino IDE.
3. Install the necessary libraries (`ThingSpeak.h`, `PubSubClient.h`).
4. Create a `secrets.h` file in the project directory with the following structure:

```cpp
// secrets.h
#define SECRET_CH1_ID_READ 123456   // Your Channel ID for Reading
#define SECRET_READ1_APIKEY "READ_API_KEY_1" // API Key for Reading
#define SECRET_WRITE1_APIKEY "WRITE_API_KEY_1" // API Key for Writing
#define SECRET_CH8_ID_WRITE 123456 // Your Channel ID for Writing
#define SECRET_WRITE8_APIKEY "WRITE_API_KEY_8" // API Key for Writing Channel 8
#define SECRET_CH_ID_SETTING 123456 // Channel ID for Settings
#define SECRET_READ_APIKEY_SETTING "READ_API_KEY_SETTING" // API Key for reading settings
```

5. Update the WiFi credentials in the main file:

```cpp
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
```

6. Upload the code to your ESP32 board.

## Working

1. The ESP32 connects to the WiFi network specified in the code.
2. It reads soil moisture data from ThingSpeak and compares it to preset high and low thresholds (configured in ThingSpeak).
3. If the moisture level is lower than the low threshold, the pump is turned ON. If the level is higher than the high threshold, the pump is turned OFF.
4. The pump and relay statuses are logged back to ThingSpeak for real-time monitoring.

## Important Functions

- **controlRelay()**: This function manages the relay's ON/OFF state based on the moisture reading.
- **controlPump()**: This function controls the water pump based on the relay's status.
- **readThingSpeakField()**: Reads data from a specific ThingSpeak field.
- **updateThingSpeak()**: Updates ThingSpeak with the current system status.

## Future Enhancements

- Adding support for multiple zones using more sensors and relays.
- Integrating a mobile app for remote control.
- Sending alerts or notifications when the pump status changes.


### Instructions for Use:
- Follow the **Software Configuration** section to set up your `secrets.h` file with your ThingSpeak channel details and WiFi credentials.
- Upload the code to your ESP32, and the system will start monitoring and controlling the pump automatically based on the moisture level from ThingSpeak. 


