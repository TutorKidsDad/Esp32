# Special Purpose Pins Identifier

This project is designed to identify and print the default special-purpose pins for the following boards:
- ESP32
- ESP8266
- Arduino Uno
- Arduino Mega

## Overview

The code initializes the serial communication and prints the default pins for SPI, I2C, UART, PWM, and Analog input based on the selected board in the Arduino IDE. 

## Supported Boards

- **ESP32**
- **ESP8266**
- **Arduino Uno**
- **Arduino Mega**

## Special Purpose Pins

### SPI Pins
- **MOSI**: Master Out Slave In
- **MISO**: Master In Slave Out
- **SCK**: Serial Clock
- **SS**: Slave Select

### I2C Pins
- **SDA**: Serial Data Line
- **SCL**: Serial Clock Line

### UART Pins
- **TX**: Transmit Pin
- **RX**: Receive Pin

### PWM Pins
- **ESP32/ESP8266**: PWM is available on most digital pins.
- **Arduino Uno**: PWM is available on pins 3, 5, 6, 9, 10, 11.
  
### Analog Input Pins
- **ESP32**: Pins 0-39 (ADC1: Pins 0-7, ADC2: Pins 8-39)
- **ESP8266**: Pin A0 (GPIO 17)
- **Arduino Uno**: Pins A0-A5
- **Arduino Mega**: Pins A0-A15

### Other Special Purpose Pins (ESP32)
- **FLASH**: GPIO 6-11
- **TOUCH**: GPIO 0, 2, 4, 12-15, 27, 32, 33
- **DAC**: GPIO 25, 26
- **CAN**: GPIO 21, 22

## Usage

1. Connect your selected board (ESP32, ESP8266, Arduino Uno, or Arduino Mega) to your computer.
2. Open the Arduino IDE and select the correct board from `Tools > Board`.
3. Upload the provided code to your board.
4. Open the Serial Monitor (set baud rate to 115200) to view the printed special-purpose pins.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

