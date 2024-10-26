# ESP32 Smart Height Measurement Project

This project demonstrates a height measurement system using an ESP32, an ultrasonic sensor, and a web-based interface. The project uses **WiFiManager** for easy network setup and **mDNS** for accessing the device via a human-readable hostname.

## Features
- **WiFiManager Integration**: Easily set up and manage Wi-Fi credentials using a captive portal.
- **mDNS Support**: Access the device using a hostname instead of an IP address (e.g., `http://sensor1.local`).
- **Ultrasonic Sensor Integration**: Measure and display distance in centimeters and inches.
- **Responsive Web Interface**: View the distance measurements on a clean web interface.

## Components Used
- **ESP32 Development Board**
- **Ultrasonic Sensor (HC-SR04 or compatible)**
- **WiFiManager Library**
- **mDNS Library**
- **NewPing Library**

## Project Structure

## Requirements
- **ESP32 Development Board**
- **Arduino IDE** with ESP32 support
- **WiFiManager Library**
- **ESPmDNS Library**
- **NewPing Library**

## Installation

1. **Clone or download** this repository to your local machine.
2. **Install the necessary libraries** in the Arduino IDE:
   - WiFiManager
   - ESPmDNS
   - NewPing

   You can install these libraries via the Arduino Library Manager.

3. **Connect your ESP32** to your computer and **upload the code** using the Arduino IDE.

## How It Works

### WiFiManager Integration
Upon boot, if no Wi-Fi credentials are stored, the ESP32 will start a Wi-Fi Access Point (AP) named **"server"** with the password **"password"**. You can connect to this AP using your phone or computer, then navigate to `http://192.168.1.1` in your web browser to enter your Wi-Fi credentials.

### Accessing the Web Interface
After connecting to your Wi-Fi network, the ESP32 will obtain an IP address and can be accessed via mDNS. You can enter `http://sensor1.local` in your web browser to view the height measurements.

### Measuring Distance
The ultrasonic sensor measures the distance, which is displayed in both centimeters and inches on the web interface. The measurements are refreshed every few seconds.

## Usage
- Connect the ultrasonic sensor to the specified GPIO pins.
- Power the ESP32 and check the Serial Monitor for connection status.
- Use the web interface to view live distance measurements.

## Troubleshooting
- If the device does not connect to Wi-Fi, check your credentials.
- Ensure the ultrasonic sensor is connected properly and functioning.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements
- [ESP32](https://www.esp32.com/)
- [WiFiManager Library](https://github.com/tzapu/WiFiManager)
- [NewPing Library](https://github.com/blakeblackshear/NewPing)


