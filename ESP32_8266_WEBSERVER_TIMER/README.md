# ESP32 Web Server with WiFiManager and mDNS

This project demonstrates how to set up an ESP32 as a web server with the capability to connect to Wi-Fi using WiFiManager. The server provides a web interface to control an output pin and adjust a timer value via a slider. The ESP32 can be accessed via a user-friendly mDNS hostname.

## Features

- **WiFiManager**: Automatically sets up Wi-Fi connections, enabling the ESP32 to act as an access point when no network credentials are available.
- **mDNS Support**: Access the web interface using a friendly hostname (`http://esp32.local`) instead of an IP address.
- **Web Interface**: Control an output pin and adjust a timer value through a simple HTML interface.
- **Real-time Interaction**: Use a slider to send timer values and toggle an output state via checkboxes.

## Requirements

- ESP32 board
- Arduino IDE or PlatformIO
- Libraries:
  - [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
  - [WiFiManager](https://github.com/tzapu/WiFiManager)
  - [AsyncTCP](https://github.com/me-no-dev/AsyncTCP)

## Installation

1. **Clone or download the repository**.
2. **Open the project in Arduino IDE**:
   - Go to `File` -> `Open` and select the downloaded project file.
3. **Install required libraries**:
   - Open the Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries...`).
   - Search for and install `ESPAsyncWebServer`, `WiFiManager`, and `AsyncTCP`.
4. **Configure WiFi Credentials**:
   - Replace the placeholders in the code for `ssid` and `password` with your network credentials.
5. **Upload the Code**:
   - Connect your ESP32 to your computer and upload the sketch.

## Usage

1. **Power the ESP32**: Connect it to a power source.
2. **Connect to Wi-Fi**:
   - If the ESP32 fails to connect to the specified Wi-Fi network, it will create a Wi-Fi access point named "ESP32-AP".
   - Connect to this network with your phone or computer to configure your desired Wi-Fi settings.
3. **Access the Web Interface**:
   - Open a web browser and go to `http://esp32.local`.
4. **Control the Output**:
   - Use the toggle switch to change the output pin state.
   - Adjust the timer value using the slider.

## Code Overview

- The main functionality resides in `setup()` and `loop()`.
- The server handles GET requests to update the output state and slider value.
- The HTML content is served from the ESP32's flash memory.

## Troubleshooting

- **Connection Issues**: If you cannot connect to the ESP32, try resetting it or re-uploading the code.
- **Web Interface Not Loading**: Ensure that your device is connected to the same network as the ESP32 or directly to the "ESP32-AP".

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [ESP32](https://www.esp32.com/)
- [Arduino](https://www.arduino.cc/)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [WiFiManager](https://github.com/tzapu/WiFiManager)

