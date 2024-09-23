# soil-institute-contactless-sensor-V2
Soil institute contactless sensor V2 where OTA is implimented via server and version check 
this is not yet implimented
Overview of the Program edited on 23rd sept 2024

This program is designed for an ESP32-based IoT system that gathers sensor data, displays it on an LCD, and communicates with a cloud service like ThingSpeak for data logging. It also implements Over-the-Air (OTA) updates, power management via deep sleep, and uses multiple peripherals including LEDs, a buzzer, and I2C sensors like AHT10/AHT20 (for temperature and humidity) and TCS34725 (for color sensing).
Key Features and Components

    WiFi and OTA Updates
        The ESP32 connects to WiFi using a captive portal for easy configuration, and checks for available OTA firmware updates. If an update is found, it is downloaded and applied.

    Sensor Data Collection
        Temperature and humidity are read from the AHT10/AHT20 sensor.
        The TCS34725 sensor is used to detect RGB colors and calculate lux (brightness) levels.
        Soil moisture and battery voltage are measured via analog pins.

    Data Upload to ThingSpeak
        Collected data (temperature, humidity, moisture, and battery voltage) is uploaded to ThingSpeak using an HTTP request.
        The program includes a periodic task to send data every 5 seconds.

    Display
        A 20x4 I2C LCD is used to show device status, sensor data, and other relevant information.
        The display mode can be toggled using interrupts, allowing for different screens (e.g., basic sensor info, color sensor data).

    Deep Sleep and Power Management
        If the battery voltage drops below 3.5V, the system enters deep sleep for 3 minutes to conserve power.
        The ESP32 wakes up from deep sleep using a timer and checks why it woke up, resuming operations accordingly.

    LED and Buzzer Control
        RGB LEDs indicate different conditions based on humidity levels.
        A buzzer is used to signal data upload success.

Breakdown of the Components
1. Main Program (ino)

    Setup Function: Initializes the display, sensors, and LEDs. Also checks the cause of the ESP32 waking up and handles WiFi connection.
    Loop Function: Handles periodic tasks like reading sensor data, updating the display, and uploading data to ThingSpeak. If the battery voltage is low, the ESP32 goes into deep sleep.

2. OTA Update (ota.h)

    Checks for OTA updates by comparing the current firmware version with the latest version available online.
    If a newer version is found, it performs an OTA update by downloading and flashing the new firmware.
    It also defines deep sleep functionality for the ESP32 to conserve power.

3. Communication (comms.h)

    Manages WiFi connectivity and data upload to ThingSpeak.
    Uploads sensor data including temperature, humidity, moisture, and battery voltage to a pre-defined URL (ThingSpeak server).
    If WiFi is not available, it attempts to reconnect using the captive portal method.

4. Display Control (display.h)

    Initializes the I2C LCD and controls what is shown on the screen.
    The screen displays sensor data such as temperature, humidity, and RGB strip color information. It also provides feedback during WiFi connection and firmware updates.
    A button can toggle between two display modes.

5. Pins and Sensor Handling (pins.h & sensors.h)

    pins.h: Defines the pin numbers for peripherals (LEDs, buzzer, sensors, etc.).
    sensors.h: Handles the initialization and reading of the AHT and TCS sensors. The readings are processed and logged in global variables.

Power Management

The program actively monitors the battery voltage (vbatt). If it detects that the voltage falls below 3.5V, it enters deep sleep for a predefined duration (3 minutes in this case) to conserve power. Upon waking, it checks whether the wake-up was caused by the deep sleep timer and resumes operations.
Example Use Case

This system could be used in a remote environmental monitoring application, where the ESP32 collects temperature, humidity, and other environmental data, uploads it to the cloud (ThingSpeak), and displays relevant information on a local LCD. The system is capable of receiving firmware updates over-the-air and can enter low-power modes to extend battery life in the field.
