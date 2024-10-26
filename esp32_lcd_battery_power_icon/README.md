
# I2C LCD Display with Custom Characters

## Project Overview

This project demonstrates how to create custom characters on an **I2C LCD display** using the **LiquidCrystal I2C** library with an **ESP32 or Arduino**. The code displays a power plug and battery icon on a 16x2 LCD screen.

## Features

- **Custom Characters**: Display a power plug and a battery icon on the LCD.
- **Simple Setup**: Uses I2C communication to interact with the LCD display.
- **Flexible Code**: Easily adaptable for other custom characters.

## Hardware Requirements

- **ESP32 or Arduino** (or any compatible microcontroller)
- **16x2 I2C LCD Display** (commonly at address 0x27)
- **Jumper wires** and a **breadboard**

## Software Requirements

- **Arduino IDE**
- **LiquidCrystal I2C Library** (Install via Library Manager)

## Circuit Diagram

### Pin Configuration

- **I2C LCD**:
  - **SDA**: Connect to I2C data line (usually pin 21 on ESP32 or A4 on Arduino Uno)
  - **SCL**: Connect to I2C clock line (usually pin 22 on ESP32 or A5 on Arduino Uno)

## Setup Instructions

1. **Connect the Hardware**:
   - Connect the I2C LCD to the I2C pins of your microcontroller.
   - Ensure the I2C address (default: `0x27`) matches your LCD.

2. **Install the Required Libraries**:
   - **LiquidCrystal I2C Library**: Go to `Sketch > Include Library > Manage Libraries` and search for `LiquidCrystal I2C`.

3. **Upload the Code**:
   - Open the **`main.cpp`** file in the Arduino IDE.
   - Compile and upload the code to your microcontroller.

## How It Works

1. **Initialization**: The LCD is initialized, and the custom characters are created.
2. **Display Icons**: The custom characters are displayed on the LCD at specific cursor positions.
3. **Loop Function**: The code in the loop does nothing in this project, as the icons are static.

## Customization

- You can modify the `batteryChar` and `powerPlugChar` arrays to create your own custom characters.
- To display additional custom characters, use `lcd.createChar()` with a unique character number (0–7).

## Troubleshooting

- **Nothing Displayed**: Check the I2C address and ensure it's correctly connected.
- **Wrong Icons**: Verify that your custom character arrays are correctly defined.

## License

This project is open-source under the [MIT License](LICENSE).

