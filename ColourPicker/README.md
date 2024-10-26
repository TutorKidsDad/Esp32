# RGB Color Picker with ESP32

## Project Overview

This project demonstrates how to create an RGB Color Picker using an **ESP32** and an **Adafruit TCS34725 RGB color sensor**. The device measures the color of real-life objects and displays the RGB values on an I2C LCD screen. An RGB LED is used to reproduce the detected color.

The project uses a **button** to trigger color readings. When pressed, the color sensor reads the RGB values of the object and updates the RGB LED to mimic the color.

## Features

- **Color Detection**: Measures RGB color values using the Adafruit TCS34725 sensor.
- **LCD Display**: Displays RGB values on a 16x2 I2C LCD screen.
- **RGB LED Reproduction**: Uses an RGB LED to visually reproduce the detected color.
- **Button Controlled**: Press a button to capture and display colors.

## Hardware Requirements

- **ESP32 Development Board**
- **Adafruit TCS34725 RGB Color Sensor** (or compatible)
- **RGB LED** (common anode or cathode, based on your choice)
- **I2C LCD Display (16x2)**
- **Push Button**
- **Resistors** (for the LEDs and button, if required)
- **Wires and Breadboard**

## Software Requirements

- **Arduino IDE** with ESP32 support
- **Adafruit TCS34725 Library** (Install via Library Manager)
- **LiquidCrystal I2C Library** (Install via Library Manager)

## Circuit Diagram

### Pin Configuration

- **ESP32 GPIO Pins**:
  - **White LED**: GPIO 15
  - **Button**: GPIO 14
  - **Red LED**: GPIO 16
  - **Green LED**: GPIO 17
  - **Blue LED**: GPIO 18
- **I2C LCD**: Connect to standard I2C pins (SDA and SCL)
- **Color Sensor**: Connect to standard I2C pins (SDA and SCL)

## Setup Instructions

1. **Connect the Hardware**:
   - Connect the RGB LED to the ESP32 GPIO pins as per the pin configuration.
   - Connect the Adafruit TCS34725 color sensor to the I2C pins of the ESP32.
   - Connect the I2C LCD display to the I2C pins of the ESP32.
   - Connect the button to a GPIO pin with a pull-down resistor if required.
   
2. **Install the Required Libraries**:
   - **Adafruit TCS34725 Library**: Go to `Sketch > Include Library > Manage Libraries` and search for `Adafruit TCS34725`.
   - **LiquidCrystal I2C Library**: Go to `Sketch > Include Library > Manage Libraries` and search for `LiquidCrystal I2C`.

3. **Upload the Code**:
   - Open the **`main.cpp`** file in the Arduino IDE.
   - Update the LED pin definitions and LCD address if required.
   - Compile and upload the code to your ESP32 board.

## How It Works

1. **Initialization**: The ESP32 initializes the I2C LCD, the color sensor, and sets up the pins for the RGB LED and the button.
2. **Button Press**: When the button is pressed, the ESP32 turns on the color sensor’s LED and reads the RGB values of the object.
3. **Display on LCD**: The RGB values are displayed on the LCD screen.
4. **Visual Representation**: The RGB LED reproduces the measured color using the detected RGB values.

## Troubleshooting

- **LCD Not Displaying Correctly**: Double-check the I2C address of the LCD and ensure it's correctly connected.
- **Color Sensor Not Detected**: Ensure the sensor is correctly connected to the I2C pins of the ESP32.
- **LED Colors Not Accurate**: Adjust the gamma correction table if the colors on the RGB LED do not match the measured colors.

## License

This project is open-source under the [MIT License](LICENSE).

