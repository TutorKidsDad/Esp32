# SMS Controlled Lamp with ESP8266/ESP32 and GSM Module

This project allows you to control a 12V lamp using SMS commands via an ESP8266 or ESP32 connected to a GSM module (SIM900). The lamp can be turned ON or OFF, and you can also check its current state by sending SMS commands.

## Features

- Turn the lamp ON or OFF using SMS.
- Query the current state of the lamp via SMS.
- Compatible with ESP8266 and ESP32 microcontrollers.
- Uses a GSM module (SIM900) for SMS communication.

## Components Required

- ESP8266 or ESP32 microcontroller
- GSM Module (e.g., SIM900)
- Relay module (for controlling the lamp)
- 12V lamp
- Power supply for the lamp
- Jumper wires


### Connections

- **GSM Module (SIM900)**
  - Connect the GSM module to the microcontroller as follows:
    - TX of GSM to RX of ESP (GPIO 3 for ESP32 or appropriate RX pin for ESP8266)
    - RX of GSM to TX of ESP (GPIO 1 for ESP32 or appropriate TX pin for ESP8266)
    - Power the GSM module using the appropriate voltage (typically 5V).
    
- **Relay Module**
  - Connect the relay control pin to pin 12 of the microcontroller.
  - Connect the 12V lamp in parallel with the relay.
  
- **Power Supply**
  - Ensure the lamp is powered by a suitable 12V supply.

## Code Overview

The code initializes the GSM module, configures it for SMS operation, and listens for incoming SMS messages. The following commands can be sent to control the lamp:

- **ON**: Turns the lamp ON.
- **OFF**: Turns the lamp OFF.
- **STATE**: Queries the current state of the lamp and sends an SMS response indicating whether it is ON or OFF.

## Setup Instructions

1. **Clone the Repository**:

2. **Install Libraries**:
   Make sure you have the necessary libraries installed in your Arduino IDE:
   - No additional libraries are needed beyond the built-in ones.

3. **Upload Code**:
   - Open the `esp32_lamp_control_via_sms.ino` file in the Arduino IDE.
   - Replace `XXXXXXXXXX` in the `sendSMS` function with your mobile number (in international format).
   - Select the appropriate board (ESP8266 or ESP32) and port in the Arduino IDE.
   - Upload the code to your microcontroller.

4. **Power Up**:
   - Connect the hardware according to the circuit diagram and power the ESP and GSM module.

## Usage

- Send an SMS with the message `ON` to turn the lamp ON.
- Send an SMS with the message `OFF` to turn the lamp OFF.
- Send an SMS with the message `STATE` to receive the current state of the lamp.

## Troubleshooting

- If the GSM module does not respond, check the power supply and connections.
- Ensure that the SIM card is properly inserted and has sufficient balance for sending and receiving SMS.
- Verify that the correct serial pins are used based on whether you're using ESP8266 or ESP32.

## License

This project is open-source and available for anyone to use or modify. Feel free to contribute or improve it!



