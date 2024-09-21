# ESP32/ESP8266 Hostname Assignment

This repository contains an Arduino sketch that demonstrates how to assign a hostname to an ESP32 or ESP8266 device and automatically detects which board is being used.

## Features

- Automatically detects whether the device is an ESP32 or ESP8266.
- Assigns a custom hostname to the device.
- Connects to a specified WiFi network.
- Prints the device type and IP address to the serial monitor.

## Requirements

- Arduino IDE (latest version)
- ESP32 or ESP8266 board package installed in the Arduino IDE
- Appropriate libraries included by the sketch

## Key Changes:

 -   Preprocessor Directives: The #if defined(ESP32) and #elif defined(ESP8266) checks which board is being compiled, allowing the code to include the appropriate WiFi library and set a board type string.
  -  Output Device Type: The device type is printed to the serial monitor during setup for easy identification.

## Instructions:

  -  Replace your_SSID, your_PASSWORD, and YourDeviceName with your actual WiFi credentials and desired hostname.
  -  Upload the code to your ESP32 or ESP8266 board, and check the serial monitor for the device type and IP address once connected.
