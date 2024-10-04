# ESP32 DHT11 Temperature & Humidity Monitoring Web Server

This project demonstrates how to build a simple temperature and humidity monitoring system using the ESP32, DHT11 sensor, and a web server to display real-time data.

## Table of Contents
1. [Overview](#overview)
2. [Hardware Components](#hardware-components)
3. [Software Libraries](#software-libraries)
4. [Working](#working)
5. [Code Explanation](#code-explanation)
6. [HTML for Web Interface](#html-for-web-interface)
7. [Circuit Diagram](#circuit-diagram)

---

## Overview
This project uses an ESP32 microcontroller to read temperature and humidity data from a DHT11 sensor and serve that data on a web page. The web page is hosted locally by the ESP32 and can be accessed via any device connected to the same network.

### Features:
- Displays real-time temperature and humidity data.
- Web page automatically refreshes every 5 seconds to update sensor data.
- Responsive design using HTML and basic CSS.

## Hardware Components
- **ESP32** – Microcontroller for WiFi and data processing.
- **DHT11 Sensor** – Used to measure temperature and humidity.
- **Jumper Wires** – To connect the components.
- **Power Source** – Can be powered via USB or a suitable battery.

## Software Libraries
The following libraries are required to run this project:

- **WiFi.h**: To establish WiFi communication.
- **WebServer.h**: To host the web server on the ESP32.
- **DHT.h**: To interface with the DHT11 sensor.

## Working
1. **ESP32 as an Access Point**: The ESP32 is configured to act as a WiFi Access Point (AP) with its own SSID and password.
2. **DHT11 Sensor Readings**: The DHT11 sensor provides temperature and humidity data, which is read and processed by the ESP32.
3. **Web Server**: The ESP32 hosts a simple web server that displays the current temperature and humidity values on a dynamically refreshing web page.
4. **Auto Refresh**: The webpage refreshes every 5 seconds to update the sensor data.

## HTML for Web Interface

The HTML page has a clean design with the Open Sans font, a minimalistic style, and uses SVG icons to represent temperature and humidity. Here's a breakdown of the design:

- **Temperature & Humidity**: Displayed with custom icons.
- **Auto-refresh**: Uses JavaScript to refresh the page every 5 seconds.
- **Responsive Design**: The design adapts to different screen sizes using simple CSS.

### Key Styles

- `.temperature`: Defines the style for the temperature value (in °C).
- `.humidity`: Defines the style for the humidity value (in %).

## Circuit Diagram

Here’s the pin connections between the ESP32 and the DHT11 sensor:

| **Component**      | **ESP32 Pin**    |
|--------------------|------------------|
| DHT11 Data Pin     | GPIO 4           |
| VCC (DHT11)        | 3.3V             |
| GND (DHT11)        | GND              |

