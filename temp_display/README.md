# Temperature Monitoring System

This project monitors temperature using a DS18B20 temperature sensor and displays the readings on a 16x2 I2C LCD. The code is compatible with ESP32, ESP8266, and Arduino Uno platforms.

## Components Required

- ESP32 or ESP8266 or Arduino Uno
- DS18B20 Temperature Sensor
- 16x2 I2C LCD Display
- 4.7k Ohm Resistor (for DS18B20)
- Breadboard and Jumper Wires

## Wiring Diagram

### DS18B20 Connections
- Connect the **VCC** pin of the DS18B20 to **3.3V** (ESP32/ESP8266) or **5V** (Arduino Uno).
- Connect the **GND** pin to **GND**.
- Connect the **DATA** pin to the defined `ONE_WIRE_BUS` pin (see code for specific pin definitions).
- Connect a **4.7k Ohm** resistor between the **VCC** and **DATA** pins.

### I2C LCD Connections
- Connect **SDA** pin of the LCD to the defined `SDA_PIN` (see code for specific pin definitions).
- Connect **SCL** pin of the LCD to the defined `SCL_PIN` (see code for specific pin definitions).
- Connect **VCC** and **GND** of the LCD to the power and ground of the microcontroller.

## Installation

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd <repository-folder>
## Installation

2. **Open the Arduino IDE.**

3. **Install required libraries:**
   - LiquidCrystal I2C
   - DallasTemperature
   - OneWire

   You can install these libraries via the Library Manager in the Arduino IDE (`Sketch > Include Library > Manage Libraries...`).

4. **Upload the code:**
   - Select the correct board from `Tools > Board`.
   - Connect your microcontroller to your computer.
   - Upload the code to your device.

## Usage

1. Open the Serial Monitor (Ctrl + Shift + M) to view temperature readings.
2. The LCD will display "HELLO" and "WORLD" during initialization and then show the current temperature reading.

## Code Overview

### Main Components

- **OneWire Setup:** Initializes the OneWire instance to communicate with the DS18B20 temperature sensor.
- **LCD Initialization:** Sets up the I2C LCD and displays a welcome message.
- **Temperature Reading:** Continuously reads the temperature from the DS18B20 sensor and updates the LCD display.


###Compatibility

###This project is compatible with:

    -ESP32
    -ESP8266
    -Arduino Uno

###Troubleshooting

    -Ensure all connections are secure and correct.
    -Double-check the pin assignments in the code according to your hardware setup.
    -Verify that the libraries are installed correctly.
