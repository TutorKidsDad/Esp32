# TCS34725 Color Sensor Example

This project demonstrates how to use the **Adafruit TCS34725** color sensor with an Arduino. The code reads the RGB values, color temperature, and illuminance (lux) from the sensor and displays the results on the Serial Monitor.

## Components Needed

- **Adafruit TCS34725 Color Sensor**
- **Arduino Board** (e.g., Arduino Uno, ESP32)
- **Jumper Wires**

## Wiring Connections

Connect the TCS34725 sensor to your Arduino as follows:

| Sensor Pin | Arduino Pin    |
|------------|----------------|
| VDD        | 3.3V DC        |
| GND        | GND            |
| SCL        | A5 (SCL)       |
| SDA        | A4 (SDA)       |

## Code Explanation

### Libraries

This code utilizes the following libraries:

- `Wire.h`: For I2C communication.
- `Adafruit_TCS34725.h`: For interfacing with the TCS34725 sensor.

### Key Functions

- `setup()`: Initializes the Serial Monitor and the TCS34725 sensor.
- `loop()`: Retrieves and displays color readings (RGB, color temperature, and lux) from the sensor every 300 milliseconds.

### Sensor Readings

The following data is printed to the Serial Monitor:

- **Color Temperature (in Kelvin)**
- **Illuminance (in Lux)**
- **Raw RGB Values** (Red, Green, Blue, Clear)

## How to Use

1. **Install Required Libraries**:
   - Ensure that you have the Adafruit TCS34725 library installed. You can install it via the Library Manager in the Arduino IDE.

2. **Upload the Code**:
   - Copy the provided code into your Arduino IDE.
   - Replace any necessary configurations if needed.
   - Upload the code to your Arduino board.

3. **Open Serial Monitor**:
   - After uploading the code, open the Serial Monitor in the Arduino IDE.
   - Set the baud rate to **115200**.

4. **Observe the Output**:
   - The sensor readings will appear in the Serial Monitor, providing real-time data about the detected color and light levels.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgements

This project is based on the Adafruit TCS34725 color sensor library and examples provided by Adafruit.


