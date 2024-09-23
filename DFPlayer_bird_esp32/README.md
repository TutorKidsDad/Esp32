# Motion-Triggered Audio Playback using ESP32 and DFPlayer Mini

## Overview

This project implements a motion-triggered audio playback system using an ESP32 microcontroller, a PIR motion sensor, an LED, and a DFPlayer Mini MP3 module. When the motion sensor detects movement, a random audio file is played from the DFPlayer Mini, and an LED lights up during the playback. The playback lasts for 15 seconds, after which the audio stops and the LED turns off.

## Components
- **ESP32**: The main microcontroller handling motion detection and audio playback control.
- **PIR Motion Sensor**: Detects motion to trigger the audio playback.
- **DFPlayer Mini**: MP3 player module to play audio files stored on an SD card.
- **LED**: Indicates motion detection and audio playback.
- **SoftwareSerial**: Used for serial communication with the DFPlayer Mini.
- **SD Card**: Stores the audio files for playback.

## Features
- Plays a random audio file from the DFPlayer Mini when motion is detected.
- Stops playback after 15 seconds.
- Uses an LED to indicate motion detection and audio playback status.
- Automatically handles DFPlayer Mini errors (e.g., if the SD card is not found).

## Wiring
- **DFPlayer Mini to ESP32:**
  - DFPlayer **RX** -> ESP32 **GPIO 17** (TX)
  - DFPlayer **TX** -> ESP32 **GPIO 16** (RX)
- **PIR Motion Sensor**: Connect to **GPIO 27** on the ESP32.
- **LED**: Connect to **GPIO 2** on the ESP32.
- **SD Card**: Insert into the DFPlayer Mini.

## Code Breakdown

### Global Variables:
- **motionPin**: Pin for the motion sensor (GPIO 27).
- **led**: Pin for the LED (GPIO 2).
- **playbackDuration**: Playback duration of 15 seconds.
- **state**: Tracks whether motion is detected (`HIGH` or `LOW`).
- **val**: Stores the current reading of the motion sensor.
- **playbackStartMillis**: Stores the timestamp when playback starts.
- **isPlaying**: Boolean flag to track if audio is currently playing.

### `setup()`:
- Initializes the motion sensor, LED, serial communication, and DFPlayer Mini.
- Sets the volume level and equalizer mode for the DFPlayer Mini.
- Displays error messages if DFPlayer Mini initialization fails.

### `loop()`:
- Continuously checks for motion sensor input.
- If motion is detected:
  - Turns on the LED.
  - Plays a random audio file if no previous motion was detected.
  - Tracks playback time and stops the audio after 15 seconds.
- If no motion is detected, turns off the LED and resets the state.

### `playRandomFile()`:
- Reads the number of audio files on the SD card.
- Selects and plays a random file.
- Displays error messages if the SD card or file count cannot be read.

## How It Works
1. The motion sensor continuously monitors for movement.
2. When movement is detected:
   - The LED turns on.
   - A random audio file is played from the DFPlayer Mini for 15 seconds.
3. After 15 seconds of playback, the audio stops and the LED turns off.
4. The system resets to detect the next motion event.

## Requirements
- Arduino IDE with ESP32 board support.
- **DFRobotDFPlayerMini** library installed from the Library Manager.

## Installation
1. Connect the hardware as described in the wiring section.
2. Load the code onto the ESP32 using the Arduino IDE.
3. Ensure the SD card in the DFPlayer Mini has audio files stored.

## License
This project is open-source and free to use under the MIT License.

