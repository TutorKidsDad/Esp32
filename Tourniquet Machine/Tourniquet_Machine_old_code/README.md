This code controls an ESP32-based machine that measures and adjusts pressure, with a timer and a web interface for remote control. Here's how it works:

### Libraries and Setup:
- **`Wire.h`** and **`LiquidCrystal_I2C.h`**: Used for communication with an I2C LCD to display pressure and status information.
- **`HX711.h`**: Used for reading pressure via the HX711 load cell amplifier.
- **`WiFi.h`, `AsyncTCP.h`, and `ESPAsyncWebServer.h`**: Handle Wi-Fi and web server functionalities.
  
### Pin Definitions:
- **Input pins** (`upPin`, `downPin`, `inflatePin`, `deflatepin`, `timerUpPin`, `timerDownPin`, `timerStartPin`): Control different aspects of the system, including pressure adjustment, inflation/deflation, and timer settings.
- **Output pins** (`pumpPin`, `valvePin`, `timerBuzzerPin`): Control the pump, solenoid valve, and a buzzer for the timer.

### Pressure Control:
- **HX711**: Used to read pressure from a load cell. `scale.get_units()` retrieves the current pressure.
- **Inflation**: When the "inflate" button is pressed, the pump turns on to increase pressure. Once the set pressure is reached, the pump turns off.
- **Deflation**: Requires pressing the "deflate" button twice. The solenoid valve opens to release pressure until it reaches the set pressure.
- **Warning**: If pressure fails to increase while inflating, the LCD displays a warning to check the system.

### Timer Control:
- **Set Timer**: The "timerUp" and "timerDown" buttons adjust the set time. The timer starts when the "timerStart" button is pressed, and a buzzer sounds until the timer finishes.
- **Display**: The timer state and pressure are shown on the I2C LCD.

### Web Interface:
- The system hosts a web page accessible via Wi-Fi. The web page allows remote control of the valve state (open/close) by toggling a switch.
- The web interface uses `AsyncWebServer` to serve the page and handle requests. The switch on the page can be toggled to turn the valve on or off by sending a request to `/update?state=1` or `/update?state=0`.

### Key Features:
1. **Pressure Adjustment**: The "up" and "down" buttons adjust the target pressure.
2. **Pump Control**: The pump inflates the system to the set pressure and turns off automatically.
3. **Valve Control**: The solenoid valve deflates the system upon user request.
4. **Debouncing**: The system debounces the buttons to prevent false triggering due to noise.
5. **LCD Display**: Shows the set and current pressure, and warnings if the pressure does not increase as expected.
6. **Web Control**: The system's valve can be controlled remotely via a web page hosted on the ESP32.

### Flow of Control:
1. **Setup**:
   - Initializes pressure sensors, LCD, Wi-Fi, and web server.
   - Connects to a Wi-Fi network and displays the IP address for web control.
   
2. **Loop**:
   - Continuously checks for button presses to adjust the set pressure, control the pump, and open/close the valve.
   - Handles timer operations and pressure monitoring.
   - Updates the web server and responds to requests to control the valve remotely.
   - Displays current pressure on the LCD screen.


