# not yet tested on hardware
# Motion-Activated Audio Playback System with MAX98357A I2S Amplifier and DFPlayer Mini

This project uses an ESP32 microcontroller to create a motion-activated audio playback system. When motion is detected via a motion sensor, a random audio file is played through a MAX98357A I2S Digital Audio Amplifier. The audio files are stored on an SD card, and a DFPlayer Mini module is used to manage the files. Additionally, an LED is used to indicate motion detection.

## Features
- Motion detection using a PIR sensor.
- Plays random audio files from an SD card upon detecting motion.
- Uses MAX98357A I2S amplifier for high-quality audio output.
- DFPlayer Mini handles file management on the SD card.
- An LED is turned on when motion is detected and audio is playing.

## Components Used
- ESP32
- MAX98357A I2S Digital Audio Amplifier
- DFPlayer Mini module
- PIR motion sensor
- LED
- SD card with audio files

## Pin Configuration
| Component              | ESP32 Pin |
|------------------------|-----------|
| PIR Motion Sensor       | GPIO 27   |
| LED                    | GPIO 2    |
| DFPlayer Mini (RX/TX)   | GPIO 16/17|
| MAX98357A I2S (BCLK)    | GPIO 26   |
| MAX98357A I2S (LRC)     | GPIO 25   |
| MAX98357A I2S (DOUT)    | GPIO 22   |

## How It Works
1. The system waits for the PIR sensor to detect motion.
2. When motion is detected, an LED is turned on, and a random audio file is selected and played from the SD card.
3. The audio is played through the MAX98357A I2S amplifier, providing high-quality sound output.
4. After 15 seconds of playback, the audio stops, and the LED turns off.
5. The system then resets and waits for the next motion event.

## Code Explanation
- **Motion Detection**: The PIR sensor is connected to GPIO 27 and reads `HIGH` when motion is detected.
- **DFPlayer Mini**: Communicates with the ESP32 over software serial (GPIO 16 for RX, GPIO 17 for TX). It manages audio files stored on the SD card.
- **MAX98357A Amplifier**: Configured using the native I2S drivers in ESP32 to output high-quality audio via GPIO 26 (BCLK), GPIO 25 (LRC), and GPIO 22 (DOUT).

## Installation
1. **Hardware Setup**: Connect the components as per the pin configuration table above.
2. **Library Dependencies**: Install the following libraries via the Arduino IDE Library Manager:
   - DFRobotDFPlayerMini
3. **Upload Code**: Upload the provided code to your ESP32.

## Usage
- Power on the ESP32 with the connected peripherals.
- When motion is detected, the LED will turn on, and a random audio file will play through the amplifier for 15 seconds.

## License
This project is licensed under the MIT License. Feel free to modify and distribute as needed.


