This code allows the operation of home appliances through voice commands via Google Assistant by communicating with the ThingSpeak MQTT broker, which in turn controls an ESP32 microcontroller connected to various appliances. Here's a breakdown of the code's functionality:

### Main Components:

- **Google Assistant**: Sends commands to control home appliances.
- **ThingSpeak MQTT**: Acts as the intermediary, receiving Google Assistant commands and passing them to the ESP32 via MQTT.
- **ESP32**: Receives MQTT messages and controls the connected appliances by switching their GPIO pins on or off.

### Key Libraries:
1. **WiFi.h**: Enables the ESP32 to connect to a Wi-Fi network.
2. **PubSubClient.h**: Allows the ESP32 to communicate with an MQTT broker (ThingSpeak in this case).

### Workflow:

#### 1. **Wi-Fi and MQTT Configuration:**
- The ESP32 connects to a Wi-Fi network using the credentials provided (`ssid` and `password`).
- It sets up an MQTT client with the details for ThingSpeak MQTT (`serverMqtt`, `portMqtt`, `userMqtt`, and `apiMqtt`). ThingSpeak uses this to communicate with the ESP32.

#### 2. **MQTT Callback (getMsg):**
- The `getMsg()` function is triggered when a message is received from the ThingSpeak MQTT server. This function:
  - Processes the incoming MQTT message.
  - Based on the content of the message (`msg`), it toggles the GPIO pins to turn on or off the connected appliances.
  
  Example:
  - If the message is `"1L1"`, the appliance connected to GPIO pin 12 is turned **ON**.
  - If the message is `"1L0"`, the appliance connected to GPIO pin 12 is turned **OFF**.

#### 3. **Control of Home Appliances:**
- The GPIO pins (12, 13, 32, 33, 25, 26, 27, 14) are associated with different appliances.
- Depending on the message received via MQTT, these pins are set to `HIGH` (turning on the appliance) or `LOW` (turning off the appliance).
  - **`1L1` to `1L0`** for GPIO 12
  - **`2L1` to `2L0`** for GPIO 13
  - **...**
  - **`8L1` to `8L0`** for GPIO 14

#### 4. **Connecting to MQTT and Subscribing to Commands:**
- The ESP32 first connects to the ThingSpeak MQTT broker. If the connection is successful, it subscribes to the appropriate channel and field on ThingSpeak, listening for commands.
  - The subscription is done using: 
    ```cpp
    client.subscribe("channels/<channel id>/subscribe/fields/field1/<Read API Key>");
    ```
  - This ensures that whenever a new message (e.g., a command to turn on/off an appliance) is published to this channel, the ESP32 receives it.

#### 5. **Network Reconnection:**
- The `loop()` function ensures that the ESP32 stays connected to the Wi-Fi and MQTT broker. If the connection is lost, it tries to reconnect.
- If the Wi-Fi or MQTT connection is dropped, it automatically reconnects and resubscribes to the MQTT channel to ensure it continues receiving commands.

### Example Use Case:
- **Google Assistant Command**: "Turn on light 1."
- **MQTT Message**: The Google Assistant triggers an action in the ThingSpeak channel, which sends an MQTT message (`1L1`) to the ESP32.
- **ESP32 Action**: The ESP32 receives the message (`1L1`) and sets GPIO 12 to `HIGH`, turning on the connected light.

### Summary:
- **Input**: Google Assistant commands sent via ThingSpeak MQTT.
- **Processing**: The ESP32 listens for commands over MQTT, processes them, and controls the corresponding GPIO pins.
- **Output**: Turns appliances on or off based on the received commands.

This setup allows for voice-controlled home automation using Google Assistant, ThingSpeak, and the ESP32.
