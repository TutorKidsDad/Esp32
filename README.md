# ESP32 Projects Collection

This repository contains a variety of simple and complex ESP32-based programs demonstrating the versatility of the ESP32 microcontroller in different use cases. Whether you're new to the ESP32 or looking for advanced applications, this collection includes something for everyone.

## Table of Contents

1. [Getting Started](#getting-started)
2. [Project List](#project-list)
3. [Hardware Requirements](#hardware-requirements)
4. [Setup and Installation](#setup-and-installation)
5. [License](#license)
6. [Contributing](#contributing)

## Getting Started

If you're new to ESP32, these projects can help you understand various aspects of programming the ESP32, such as Wi-Fi management, sensor integration, file handling, and more. Each project is contained in its own folder with the corresponding source code and documentation.

### Prerequisites

- Arduino IDE (or PlatformIO) installed
- ESP32 board package installed in Arduino IDE
- Basic knowledge of microcontroller programming

## Project List

Here’s a brief description of each project:

### Simple Projects

1. **Wi-Fi Connection Test**  
   Connects the ESP32 to a Wi-Fi network and prints the IP address.
   
2. **Blink LED**  
   A simple program that blinks an LED connected to the ESP32.

3. **Analog Sensor Reading**  
   Reads data from an analog sensor using the ESP32’s ADC and prints the value to the serial monitor.

4. **Basic Web Server**  
   Hosts a basic webpage on the ESP32, allowing users to interact with it.

### Complex Projects

1. **OTA Update with Sensor Data**  
   Allows Over-the-Air (OTA) updates and collects temperature and humidity data from a DHT11 sensor.

2. **ESP32-CAM Bird Detection**  
   Uses the ESP32-CAM to detect birds and triggers an alarm when a bird is detected in the camera frame.

3. **SD Card File Handling**  
   Demonstrates reading and writing files to an SD card connected to the ESP32.

4. **MQTT Data Publishing**  
   Publishes sensor data to an MQTT broker for IoT applications.

5. **ESP32 Power Management**  
   Demonstrates deep sleep modes and low-power configurations for battery-powered ESP32 applications.

## Hardware Requirements

- ESP32 Development Board
- DHT11/DHT22 Sensor (for temperature and humidity projects)
- MicroSD Card Reader (for SD card file handling)
- ESP32-CAM module (for bird detection)
- LEDs, resistors, and jumper wires
- Breadboard and power supply

## Setup and Installation

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/your_username/esp32-projects-collection.git
   cd esp32-projects-collection

2. **Open the Sketch**:
   - Navigate to the folder of the project you want to run.
   - Open the `.ino` file (or main file) in your preferred development environment:
     - **Arduino IDE**: File > Open > Select the `.ino` file.
     - **PlatformIO**: Open the project folder in your PlatformIO workspace.

3. **Configure Settings**:
   - Depending on the project, you may need to configure some settings before uploading the code to your ESP32:
     - **Wi-Fi Credentials**: If the project involves Wi-Fi, update the `SSID` and `password` in the code with your network details.
     - **Pin Definitions**: Make sure the GPIO pin definitions match your hardware connections.
     - **Sensor/Module Setup**: If you're using sensors (like DHT11) or other modules (like an SD card reader), configure the pins and any required libraries in the code.

4. **Install Required Libraries**:
   - Some projects may require external libraries. Install these in the Arduino IDE or PlatformIO:
     - **Arduino IDE**:
       - Go to Sketch > Include Library > Manage Libraries.
       - Search for the required library (e.g., `DHT Sensor Library`, `SD`, `ESPAsyncWebServer`), and install it.
     - **PlatformIO**:
       - Add the required library to the `platformio.ini` file under `lib_deps` or install it using the PlatformIO Library Manager.

5. **Upload the Code**:
   - Connect your ESP32 to your computer using a USB cable.
   - In the Arduino IDE, select the appropriate ESP32 board by going to Tools > Board > ESP32 Dev Module (or your specific ESP32 board).
   - Select the correct COM port under Tools > Port.
   - Click the **Upload** button to compile and upload the sketch to your ESP32.
   - In **PlatformIO**, use the "Upload" button or run the command `pio run --target upload`.

6. **Open the Serial Monitor**:
   - Once the code is uploaded, open the Serial Monitor to see output from the ESP32:
     - **Arduino IDE**: Tools > Serial Monitor.
     - **PlatformIO**: Use the built-in Serial Monitor or run `pio device monitor`.
   - Set the baud rate to `115200` (or as specified in the code) to view the debug information.

## Example:

If you are using the **Basic Web Server** project, the output in the Serial Monitor will display the ESP32's IP address. Enter this IP address in your web browser to interact with the ESP32-hosted webpage.

## Troubleshooting

1. **Connection Issues**:
   - **ESP32 Not Detected**:
     - Ensure that you are using a **data-capable USB cable**. Some USB cables only support charging and won't allow data transfer.
     - Check if the correct COM port is selected in the Arduino IDE or PlatformIO:
       - **Arduino IDE**: Go to Tools > Port, and select the correct port.
       - **PlatformIO**: Ensure that the correct `upload_port` is specified in the `platformio.ini` file.
     - Try **reconnecting** the ESP32 or using a different USB port on your computer.
     - Install the latest **ESP32 drivers** if your device is still not detected.

2. **Library Errors**:
   - **Missing Libraries**:
     - Ensure that all necessary libraries are installed in the Arduino IDE or PlatformIO. If a specific library is missing, the compiler will show an error message with the name of the missing library.
     - For Arduino IDE, go to **Sketch > Include Library > Manage Libraries** and install the required library.
     - In **PlatformIO**, add missing libraries to your `platformio.ini` file under the `[env]` section:
       ```ini
       lib_deps =
           library_name
       ```
   - **Library Version Conflicts**:
     - Sometimes, specific projects require a particular version of a library. Make sure you're using the correct version by checking the documentation of the project.

3. **Upload Failures**:
   - **Timeout Error**:
     - If the sketch fails to upload and shows a timeout error, try holding down the **BOOT** button on the ESP32 while the upload is in progress. Release the button when the upload starts.
     - Ensure that the correct **board** is selected under **Tools > Board** in the Arduino IDE. For PlatformIO, verify the correct `board` is set in the `platformio.ini` file:
       ```ini
       [env:esp32dev]
       platform = espressif32
       board = esp32dev
       framework = arduino
       ```
   - **Port Already in Use**:
     - If you see an error saying the port is busy or already in use, close any other application (such as Serial Monitor or another terminal) that might be using the port.
     - Restart the Arduino IDE or PlatformIO if the problem persists.

4. **Wi-Fi Connection Problems**:
   - **Incorrect Wi-Fi Credentials**:
     - Double-check the **SSID** and **password** that you entered in the code. Ensure there are no extra spaces or hidden characters.
     - Verify that your ESP32 is within range of the Wi-Fi router.
   - **IP Address Not Assigned**:
     - If the ESP32 cannot get an IP address, ensure your network’s DHCP server is active. You can also try assigning a static IP address in the code:
       ```cpp
       IPAddress local_IP(192, 168, 1, 184);
       IPAddress gateway(192, 168, 1, 1);
       IPAddress subnet(255, 255, 255, 0);
       WiFi.config(local_IP, gateway, subnet);
       ```

5. **Serial Monitor Not Showing Output**:
   - Make sure the **baud rate** in the Serial Monitor matches the one set in the code (usually `115200`):
     - **Arduino IDE**: Go to Tools > Serial Monitor and select the correct baud rate.
     - **PlatformIO**: Run the `pio device monitor` command with the correct baud rate, for example:
       ```bash
       pio device monitor --baud 115200
       ```

6. **Unexpected Behavior or Crashes**:
   - **Outdated ESP32 Core**:
     - Ensure you are using the latest ESP32 core. You can update it in Arduino IDE by going to **Tools > Board > Boards Manager**, searching for `esp32`, and clicking **Update**.
     - In PlatformIO, ensure you are using the latest version of the **espressif32** platform:
       ```bash
       pio update
       ```
   - **Memory Issues**:
     - If your ESP32 runs out of memory, try to optimize the code by reducing the size of variables, arrays, or data buffers.

If you encounter other issues, refer to the project-specific documentation or the official ESP32/ESP8266 community forums for additional support.

