# Motion-Activated Audio Playback with Potentiometer Control

## Overview
This project uses an ESP32 microcontroller, a motion sensor, an LED, and a DFPlayer Mini module to create a motion-activated audio playback system. The playback duration is adjustable via a potentiometer, allowing you to set how long the audio plays when motion is detected.

## Components
- **ESP32**: Main microcontroller
- **DFPlayer Mini**: Audio playback module
- **Motion Sensor**: Detects motion
- **LED**: Indicates motion detection
- **Potentiometer**: Controls playback duration
- **Speaker**: For audio output

## Wiring Diagram
- **Motion Sensor**: Connect to GPIO 27 --Motion Sensor	GPIO 27	Connect VCC to 3.3V, GND to GND
- **LED**: Connect to GPIO 2,Connect the anode to GPIO 2, cathode to GND (use a resistor if necessary)
- **Potentiometer**: Connect middle pin to GPIO 34, one side to GND, the other to VCC (3.3V)
- **DFPlayer Mini**:
  - RX to GPIO 16
  - TX to GPIO 17
  - Connect speaker and power according to the DFPlayer Mini specifications


## Code Explanation
The code initializes the motion sensor and DFPlayer Mini. When motion is detected, it plays a random audio file for a duration determined by the potentiometer setting.

### Key Functions
- **`setup()`**: Initializes pins and sets up the DFPlayer Mini.
- **`loop()`**: Continuously checks for motion detection and handles playback logic.
- **`playRandomFile()`**: Plays a random audio file from the SD card.
- **`readPotentiometer()`**: Reads the potentiometer value and maps it to a playback duration (1 to 30 seconds).

## Installation
1. Connect all components as described in the wiring diagram.
2. Upload the provided code to your ESP32 using the Arduino IDE.
3. Make sure to include the necessary libraries:
   - `DFRobotDFPlayerMini`
   - `SoftwareSerial`
4. Place audio files (in MP3 format) on the SD card used with the DFPlayer Mini.

## Usage
- Adjust the potentiometer to set the desired playback duration.
- Move in front of the motion sensor to trigger audio playback. The LED will light up during playback.
- The audio will stop automatically after the set duration.

## Troubleshooting
- If the DFPlayer Mini does not initialize, check the connections.
- Ensure that audio files are correctly formatted and placed in the root directory of the SD card.
- If the motion sensor does not detect motion, verify its wiring and functionality.

## License
This project is open-source. Feel free to modify and use it in your projects!

## Acknowledgments
- [Arduino](https://www.arduino.cc/)
- [DFRobot](https://www.dfrobot.com/)

