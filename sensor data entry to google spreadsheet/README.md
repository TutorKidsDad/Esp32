This code is designed to connect an ESP8266 or ESP32 microcontroller to a WiFi network, read button presses, and send data to a Google Spreadsheet via a Google Apps Script (GAS) Web App URL. It uses HTTPS to securely transmit data over the internet.

Here’s a detailed explanation of what the code does:

### 1. **WiFi Connection**
   - The code uses the ESP8266 or ESP32 to connect to a WiFi network. Depending on which microcontroller you are using, it imports the appropriate libraries for the WiFi connection:
     - For ESP8266: `#include <ESP8266WiFi.h>`
     - For ESP32: `#include <WiFi.h>`
   
   - The code connects to a WiFi network using the provided SSID and password, which you need to define in the `ssid` and `password` variables.

### 2. **Button Press Detection**
   - The button is connected to a specific GPIO pin (`buttonPin`), which is monitored for changes (presses).
   - The code includes a debounce mechanism to ensure that button presses are registered accurately and that noise is ignored. Debouncing is implemented using a delay (`debounceDelay`) and checking how long the button state has remained stable before recognizing it as an actual press.
   - When the button is pressed, a signal is sent to trigger the `sendData()` function, which sends temperature and humidity data to Google Sheets.

### 3. **Reading Data from Serial Input**
   - In addition to detecting button presses, the code reads numeric data from the serial input. It builds a string (`readString`) from the incoming serial data and then converts this string into integers.
   - These integers represent different sensor values (e.g., temperature, humidity) that are used when calling the `sendData()` function.

### 4. **Data Transmission to Google Sheets**
   - The main purpose of the code is to send sensor data to a Google Spreadsheet via a Google Apps Script (GAS) Web App. The GAS Web App expects parameters in a URL and updates the spreadsheet accordingly.
   - The `sendData()` function sends a **GET request** to the Google Apps Script Web App. It constructs a URL that includes parameters for temperature (`temperature`), humidity (`humidity`), and other sensor readings. These values are transmitted to the Google Spreadsheet for storage.
   
   - The URL has the following format:
     ```
     https://script.google.com/macros/s/<GAS_ID>/exec?temperature=xx&humidity=xx
     ```

   - In the code, the URL is constructed dynamically based on the sensor values passed to the `sendData()` function. The values are appended as parameters in the GET request.
   
### 5. **TLS Connection**
   - The code uses a secure HTTPS connection to send data. For this, it uses the `WiFiClientSecure` class:
     - On ESP8266, it checks the SSL certificate using a SHA1 fingerprint.
     - On ESP32, it skips certificate validation using `client.setInsecure()` (although it is recommended to use proper certificate validation for production environments).

### 6. **Sending Test Data**
   - On startup, the code sends a set of test data using the `sendData(200,201,202,203,204,205,206,2007)` function. This is primarily for testing the connection and functionality.

### 7. **Handling HTTP Response**
   - Once the data is sent, the ESP waits for a response from the server.
   - It reads the response and checks if the string `{"state":"success"}` is received, which indicates that the data was successfully stored in Google Sheets.

### 8. **Debounce Logic**
   - The debounce mechanism ensures that button presses are accurately registered, avoiding issues with mechanical bouncing (noise) from the button. The code only triggers the data send if the button has been in a stable state for longer than the debounce delay.

---

### Flow of the Code:
1. **Setup**:
   - The ESP connects to the WiFi network.
   - Sends a test set of data to Google Sheets via an HTTPS request.

2. **Loop**:
   - The ESP constantly checks the button state.
   - If a button press is detected (debounced), it sends sensor data (temperature and humidity) to Google Sheets.
   - It also listens for input from the serial monitor. If serial input is received, it processes the data and sends it to Google Sheets.

---

### Key Variables:
- `ssid` and `password`: Store the WiFi credentials.
- `buttonPin`: The pin connected to the button.
- `host`: The URL (Google Apps Script host) where data is sent.
- `GAS_ID`: The unique Google Apps Script Web App ID.
- `fingerprint`: The SSL certificate fingerprint for ESP8266 (not used for ESP32 in this code).

### Functions:
- `setup()`: Initializes the WiFi connection and sends test data.
- `loop()`: Monitors button state and serial input to trigger data transmission.
- `sendData()`: Constructs and sends the GET request to Google Sheets, transmitting sensor data over a secure connection.

### What the Code Accomplishes:
The code allows the ESP8266/ESP32 to act as a data-logging device. It reads button inputs or serial data (sensor readings) and sends this data to a Google Spreadsheet via a secure HTTPS connection. This is a basic Internet of Things (IoT) setup where data can be remotely stored and monitored using Google Sheets.
