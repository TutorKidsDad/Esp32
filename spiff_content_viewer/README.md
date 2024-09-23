# ESP32 SPIFFS File System Example

This project demonstrates how to use the SPIFFS (SPI Flash File System) on the ESP32 to read and display the content of a file stored in the flash memory.

## Project Overview

The code reads the contents of a file (`test_example.txt`) stored in the ESP32's SPIFFS file system and prints the file content to the serial monitor.

## Requirements

- ESP32 development board
- Arduino IDE or PlatformIO
- SPIFFS library (comes pre-installed with ESP32 board support)

## Code Explanation

1. **Initializing SPIFFS**:  
   The SPIFFS file system is initialized using `SPIFFS.begin()`. If there is an issue mounting the file system, an error message is printed.

2. **Reading the File**:  
   The file `/test_example.txt` is opened in read mode. If the file is successfully opened, its contents are read and printed to the serial monitor. If the file cannot be opened, an error message is printed.

3. **Closing the File**:  
   After reading the file, it is closed to free up resources.

## How to Use

1. **Setup SPIFFS in Arduino IDE**:
   - In the Arduino IDE, ensure you have the ESP32 board support installed. 
   - Go to `Tools > ESP32 Data Upload` to upload files to SPIFFS (for PlatformIO, use the `uploadfs` command).

2. **Create a File in SPIFFS**:
   - Create a text file named `test_example.txt` and add some sample text.
   - Upload the file to SPIFFS using the file system uploader tool.

3. **Upload the Code**:
   - Compile and upload the provided code to your ESP32.

4. **View Output**:
   - Open the serial monitor (`115200 baud`) to see the contents of the file printed.

## Example Output
File Content: This is a test file stored in SPIFFS!

## Important Functions

- `SPIFFS.begin(true)`: Initializes the SPIFFS file system. If `true`, it will format the file system if it is not already formatted.
- `SPIFFS.open("/test_example.txt", FILE_READ)`: Opens the file in read mode.
- `file.read()`: Reads one byte from the file.
- `file.available()`: Returns the number of bytes available for reading.

## License

This project is open-source, and you are free to modify and use it in your own projects.

## Credits

Original code by [Rui Santos](https://randomnerdtutorials.com).


