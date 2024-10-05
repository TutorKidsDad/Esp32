# Smart Height Measurement with ESP32

This project utilizes an ESP32 microcontroller and an ultrasonic sensor (HC-SR04) to measure distances and display the readings in both centimeters and inches on a web server interface.

## Components Used
- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- Breadboard and Jumper Wires

## Features
- Measures distance using an ultrasonic sensor.
- Hosts a web server to display measurements in real-time.
- Refreshes the web page every 5 seconds to update distance readings.
- Displays the distance in both centimeters and inches.

## Code Overview

The main code is written in C++ and utilizes the following libraries:
- `WiFi.h`: For managing Wi-Fi connections.
- `WebServer.h`: For creating the web server.
- `NewPing.h`: For managing the ultrasonic sensor.

### Setup Instructions

1. **Install the Required Libraries**
   - Make sure you have the `WiFi`, `WebServer`, and `NewPing` libraries installed in your Arduino IDE.

2. **Configure Wi-Fi Credentials**
   - Open the code and replace the `ssid` and `password` variables with your desired Wi-Fi credentials:
   ```cpp
   const char* ssid = "server";  // Enter SSID here
   const char* password = "password";  // Enter Password here
## Upload the Code
Connect your ESP32 to the computer and upload the code using the Arduino IDE.

## Access the Web Interface
1. Connect your computer or mobile device to the same Wi-Fi network as the ESP32.
2. Open a web browser and navigate to the IP address: `192.168.1.1` (or the IP you set).

## Code Explanation

### Key Functions
- `setup()`: Initializes serial communication, sets pin modes, connects to Wi-Fi, and starts the web server.
- `loop()`: Continuously handles client requests to the web server.
- `handle_OnConnect()`: Handles incoming requests, triggers the ultrasonic sensor, and sends the distance data back to the client.
- `handle_NotFound()`: Returns a 404 error for any requests that do not match defined routes.
- `SendHTML()`: Constructs and returns the HTML page displaying the distance measurements.

### Pin Configuration
- `trigPin`: Pin connected to the HC-SR04 trigger.
- `echoPin`: Pin connected to the HC-SR04 echo.

## Example Output
The web interface displays the distance in two formats:
- Distance in centimeters (cm)
- Distance in inches (in)

Both values are updated every 5 seconds for real-time monitoring.

## Troubleshooting
- **ESP32 not connecting to Wi-Fi**: Check the SSID and password values.
- **No distance readings**: Ensure the ultrasonic sensor is connected properly to the specified pins.
- **Web interface not loading**: Verify that your device is connected to the correct network.

## License
This project is open-source. Feel free to modify and use it as you like.

