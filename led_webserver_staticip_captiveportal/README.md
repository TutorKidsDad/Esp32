
# ESP32/ESP8266 LED Control Web Server

This project creates a simple web server using an ESP32 or ESP8266 module, allowing you to control two LEDs through a web interface. The server operates in Access Point (AP) mode, enabling direct connections without the need for a local network.

## Features

- Control two LEDs via a web interface
- View the current state of each LED (ON/OFF)
- Connect to the server using any device with Wi-Fi capabilities

## Hardware Required

- ESP32 or ESP8266 development board
- Two LEDs
- Two current-limiting resistors (e.g., 220 ohms)
- Jumper wires
- Breadboard (optional)

## Wiring Diagram

```plaintext
ESP32/ESP8266       LED
------------------------------
GPIO 4 (D4)  ---->  LED1 (+)
GND          ---->  LED1 (-)
  
GPIO 5 (D5)  ---->  LED2 (+)
GND          ---->  LED2 (-)
```

## Software Requirements

- Arduino IDE
- ESP32/ESP8266 Board package for Arduino IDE
- Required libraries: `WiFi` and `WebServer`

## Installation

1. **Clone or download the repository:**
   ```bash
   git clone https://github.com/yourusername/your-repository.git
   cd your-repository
   ```

2. **Open the Arduino IDE and install the required board package:**
   - Go to `File` > `Preferences`.
   - In the "Additional Boards Manager URLs" field, add:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```

3. **Install the necessary libraries:**
   - Go to `Sketch` > `Include Library` > `Manage Libraries`.
   - Search for and install the `WiFi` and `WebServer` libraries if they are not already installed.

4. **Configure the Code:**
   - Open the main code file (e.g., `main.ino`).
   - Update the `ssid` and `password` variables with your desired Access Point credentials:
     ```cpp
     const char* ssid = "server";  
     const char* password = "password";  
     ```

5. **Upload the Code:**
   - Select your board and port in the Arduino IDE.
   - Upload the code to your ESP32 or ESP8266.

## Accessing the Web Server

1. **Connect your device to the ESP32/ESP8266 AP:**
   - SSID: `server`
   - Password: `password`

2. **Open a web browser and navigate to:**
   ```
   http://192.168.1.1
   ```

3. **Control the LEDs:**
   - Click on the ON/OFF buttons to change the state of each LED.

## Code Structure

- `setup()`: Initializes the serial communication, sets up the LED pins, and configures the web server.
- `loop()`: Continuously handles client requests and updates the LED states based on the current status.
- `handle_OnConnect()`: Serves the main HTML page and initializes the LED states to OFF.
- `handle_led1on()`, `handle_led1off()`, `handle_led2on()`, `handle_led2off()`: Handle the requests to turn the LEDs ON or OFF.
- `handle_NotFound()`: Returns a 404 error for any unknown route.
- `SendHTML()`: Generates the HTML content for the web page dynamically based on the LED states.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Author

Modified by: **Swapnil Karambelkar**

Original project by: **Rui Santos**  
Complete project details at: [Random Nerd Tutorials](https://randomnerdtutorials.com)

---

Feel free to contribute to this project by submitting issues or pull requests.
```

