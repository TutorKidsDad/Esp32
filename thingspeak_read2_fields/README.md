
# ESP32/ESP8266 ThingSpeak Data Fetching Example

This project demonstrates how to connect an ESP32 or ESP8266 to Wi-Fi and fetch data from ThingSpeak using the HTTP GET request method. The fetched data is in JSON format, which is then parsed to extract specific field values.

## Features
- Connects to a Wi-Fi network.
- Fetches the latest data from a specified ThingSpeak channel.
- Parses JSON data to extract field values.
- Prints the values to the Serial Monitor.

## Required Libraries
- **WiFi.h**: For handling Wi-Fi connectivity.
- **HTTPClient.h**: To send HTTP requests.
- **ArduinoJson.h**: To parse JSON data.

## Installation
1. **Clone or Download the Repository**
   Clone this repository to your local machine or download it as a ZIP file.

2. **Install Required Libraries**
   Ensure you have the required libraries installed via the Arduino Library Manager:
   - `WiFi`
   - `HTTPClient`
   - `ArduinoJson`

3. **Set Up Wi-Fi Credentials**
   Replace the placeholders for Wi-Fi and ThingSpeak credentials in the code:
   
   #define WIFI_SSID "your_SSID"
   #define WIFI_PASS "your_PASSWORD"
   #define THINGSPEAK_CHANNEL_ID "your_channel_id"
   #define THINGSPEAK_API_KEY "your_api_key"
   

## Code Explanation

### `connectToWiFi()`
This function handles connecting to the specified Wi-Fi network. It continuously attempts to connect until successful, providing feedback in the Serial Monitor.

### `fetchDataFromThingSpeak()`
This function sends an HTTP GET request to the ThingSpeak API to fetch the latest feed data from the specified channel. It constructs the request URL using the channel ID and API key.

- **HTTP GET Request**: Sends the request and checks the HTTP response code.
- **JSON Parsing**: If the response is successful, it parses the JSON payload to extract values from `field1` and `field2`, which are stored in the respective float variables.

### `setup()`
- Initializes the Serial Monitor for debugging output.
- Calls the `connectToWiFi()` function to establish a connection.
- Calls the `fetchDataFromThingSpeak()` function to retrieve and display the initial data.

### `loop()`
In this example, the `loop()` function fetches data from ThingSpeak every 30 seconds. You can adjust the delay as needed.

## Example Output
Upon successful execution, the Serial Monitor will display:

Connecting to WiFi...
Connected!
Fetching data from URL: http://api.thingspeak.com/channels/your_channel_id/feeds/last.json?api_key=your_api_key
HTTP GET request code: 200
Received payload:
{
  "field1": value,
  "field2": value
}
Field1: value
Field2: value

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements
This code is based on examples provided by various online Arduino tutorials and documentation.

