# ESP32 Smart Home Automation with Alexa and Physical Buttons

This project demonstrates how to control home appliances using an ESP32 microcontroller. It integrates **WiFiManager** to handle Wi-Fi connections, **mDNS** for easy network access, **ESPalexa** for Alexa voice control, and **AceButton** for physical button interactions.

## Features

- **Alexa Voice Control**: Control up to 8 devices using Alexa voice commands.
- **Manual Control**: Use physical buttons to toggle device states.
- **WiFiManager**: Allows users to easily configure Wi-Fi credentials through a captive portal.
- **mDNS**: Access the ESP32 using a human-readable hostname instead of an IP address.

## Project Components

- **ESP32 microcontroller**
- **Relays** connected to appliances (e.g., lamps, lights, etc.)
- **Physical Buttons** for manual control of appliances
- **Alexa Echo Device** (or other Alexa-enabled devices)

## Schematic

The ESP32 is connected to relays that control different home appliances. Each relay is associated with a physical button. When the ESP32 receives commands from Alexa or detects a button press, it toggles the corresponding relay.

The ESP32 also runs a Wi-Fi captive portal, allowing users to set up Wi-Fi credentials if the connection is lost or needs updating.

## Requirements

- **ESP32 Development Board**
- **Relay Module** (at least 8 channels for controlling up to 8 appliances)
- **Physical Buttons** (connected to ESP32 GPIO pins)
- **LED** for Wi-Fi status indication

## Libraries Used

- **WiFiManager**: To manage Wi-Fi connections via a configuration portal
- **ESPalexa**: For Alexa integration
- **AceButton**: For handling physical button presses
- **ESPmDNS**: To set up mDNS (multicast DNS) for easy access

## Pin Configuration

- **Relay Pins**: 23, 22, 21, 19, 18, 5, 25, 26
- **Button Pins**: 13, 12, 14, 27, 33, 32, 15, 4
- **Wi-Fi Status LED**: Pin 2

## Setup Instructions

### Step 1: Install the Required Libraries

You need to install the following libraries in the Arduino IDE:
- **WiFiManager** (available in the Library Manager)
- **ESPalexa** (available in the Library Manager)
- **AceButton** (available in the Library Manager)
- **ESPmDNS** (available in the Library Manager)

### Step 2: Configure Wi-Fi Manager Portal

When the ESP32 cannot connect to the saved Wi-Fi network, it will create an access point named **ESP32-Setup** with the password **password123**. Connect to this network and open a browser to `192.168.4.1` to set your Wi-Fi credentials.

### Step 3: Upload the Code

1. Open the Arduino IDE and copy the code below into a new sketch:
   ```cpp
   // Full code goes here (as provided above)
   ```
2. Modify the **Device Names** and **Wi-Fi Credentials** as needed.
3. Upload the sketch to your ESP32.

### Step 4: Access Using mDNS

Once connected to the Wi-Fi, you can access the ESP32 using the hostname `http://myesp32.local`. Ensure your computer or mobile device is on the same Wi-Fi network for this to work.

### Step 5: Configure Alexa

1. In the Alexa app, add a new device and scan for devices. The ESP32 should be detected automatically.
2. You can control the devices using the assigned device names (e.g., "Turn on Study Lamp").

### Example Code Overview

The code contains:

1. **Wi-Fi Setup with WiFiManager**:
   Uses WiFiManager to provide a configuration portal in case the ESP32 is unable to connect to a saved Wi-Fi network.

2. **mDNS Setup**:
   Allows users to access the ESP32 with a hostname like `http://myesp32.local` instead of using an IP address.

3. **Relay Control Functions**:
   Uses an array of relay pins to toggle appliances.

4. **Alexa Integration with ESPalexa**:
   Associates device names with relay control functions, allowing Alexa to turn appliances on or off.

5. **Physical Button Handling with AceButton**:
   Detects button presses and toggles the associated relay pin.

### Code Breakdown

#### WiFiManager Integration

```cpp
boolean connectWifi() {
  WiFiManager wm; // Create WiFiManager instance
  wm.setMinimumSignalQuality(30); // Set minimum quality level

  if (!wm.autoConnect("ESP32-Setup", "password123")) {
    Serial.println("Failed to connect, rebooting...");
    delay(3000);
    ESP.restart();
    return false;
  }
  return true;
}
```

#### mDNS Setup

```cpp
if (!MDNS.begin(mdnsName)) {
  Serial.println("Error setting up mDNS responder!");
} else {
  Serial.println("mDNS responder started. Visit http://myesp32.local in your browser.");
}
```

#### Device Control

```cpp
void setDeviceState(uint8_t deviceIndex, boolean state) {
  digitalWrite(RelayPins[deviceIndex], state ? LOW : HIGH);
  Serial.print("Device ");
  Serial.print(DeviceNames[deviceIndex]);
  Serial.println(state ? " ON" : " OFF");
}
```

### Usage

- **Alexa Voice Commands**: "Alexa, turn on Study Lamp" will turn on the relay connected to the first device.
- **Physical Button Control**: Pressing a button will toggle the state of the corresponding relay.

## Troubleshooting

- **Wi-Fi Not Connecting**: If the ESP32 fails to connect to Wi-Fi, check the serial monitor for error messages. You can also reset the ESP32 to clear the saved credentials and force the configuration portal to open.
- **Alexa Device Not Found**: Ensure your ESP32 and Alexa device are on the same Wi-Fi network. Restart your ESP32 and rescan for devices in the Alexa app.

## Future Enhancements

- Add support for more devices or sensors.
- Implement additional modes for manual control.
- Enhance security by adding password protection to the configuration portal.

## License

This project is licensed under the MIT License. See the `LICENSE` file for more information.

---

