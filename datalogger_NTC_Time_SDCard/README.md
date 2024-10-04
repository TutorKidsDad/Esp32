# BME280 SD Card Logger

This project is an ESP32-based data logger that reads temperature, humidity, and pressure from a BME280 sensor and logs the data to an SD card. It also fetches the current epoch time from an NTP server for accurate timestamping of the recorded data.

## Features

- Reads data from the BME280 sensor
- Logs sensor data (temperature, humidity, pressure) to an SD card
- Fetches current epoch time using NTP
- Creates a CSV file (`data.txt`) on the SD card if it doesn't exist
- Appends new sensor readings to the CSV file

## Components Required

- ESP32 Development Board
- BME280 Sensor
- Micro SD Card Module
- Micro SD Card
- Jumper Wires

## Libraries Used

- `FS.h`
- `SD.h`
- `SPI.h`
- `Adafruit_Sensor.h`
- `Adafruit_BME280.h`
- `WiFi.h`
- `time.h`

## Wiring Diagram

| ESP32 Pin   | BME280 Pin | SD Card Module Pin |
|-------------|------------|--------------------|
| 3.3V       | VCC        | 3.3V               |
| GND        | GND        | GND                |
| SDA        | SDA        | SDA                |
| SCL        | SCL        | SCL                |
| GPIO23     |            | MOSI               |
| GPIO19     |            | MISO               |
| GPIO18     |            | SCK                |

## Installation

1. Clone or download this repository to your local machine.
2. Open the `.ino` file in the Arduino IDE.
3. Install the required libraries using the Library Manager in the Arduino IDE.
4. Replace the `ssid` and `password` variables with your WiFi credentials.
5. Connect the ESP32 to your computer and upload the code.

## Usage

1. Once the code is uploaded, open the Serial Monitor (set to 115200 baud).
2. The ESP32 will connect to the WiFi network and initialize the BME280 sensor and SD card.
3. Every 30 seconds, it will read the sensor data and append it to the `data.txt` file on the SD card.
4. You can access the data file by removing the SD card and reading it on your computer.

## Troubleshooting

- Ensure that the BME280 sensor is properly connected and functioning.
- Check the wiring for the SD card module.
- Ensure the SD card is formatted correctly (FAT32 is recommended).

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

This project was inspired by tutorials from [Random Nerd Tutorials](https://RandomNerdTutorials.com).

