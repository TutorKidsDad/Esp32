#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Descriptive Input Names
const char* STRING_INPUT = "input_string";   // String type input
const char* INTEGER_INPUT = "input_integer"; // Integer type input
const char* FLOAT_INPUT = "input_float";     // Float type input
/*
Here are examples of the types of inputs that would correspond to the descriptive input names in the form:

1. **String Input** (`input_string`):
   - This input is for entering a sequence of characters (a string). The user could enter text like:
     - Example: `"Hello World"`
     - Example: `"ESP32 Server"`
     - Example: `"WiFi Network"`

2. **Integer Input** (`input_integer`):
   - This input is for entering whole numbers (integers). The user could input values like:
     - Example: `123`
     - Example: `42`
     - Example: `-10`

3. **Float Input** (`input_float`):
   - This input is for entering decimal numbers (floating-point numbers). The user could input values like:
     - Example: `3.14`
     - Example: `-0.99`
     - Example: `100.5`

These inputs will be parsed and processed accordingly in the backend when the form is submitted to the ESP32 server.
 */

// HTML web page to handle the 3 input fields
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP32 HTML Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head><body>
  <form action="/get">
    Enter string input: <input type="text" name="input_string">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter integer input: <input type="text" name="input_integer">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Enter float input: <input type="text" name="input_float">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle the input from the form
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;

    // Handle string input
    if (request->hasParam(STRING_INPUT)) {
      inputMessage = request->getParam(STRING_INPUT)->value();
      inputParam = STRING_INPUT;
    }
    // Handle integer input
    else if (request->hasParam(INTEGER_INPUT)) {
      inputMessage = request->getParam(INTEGER_INPUT)->value();
      inputParam = INTEGER_INPUT;
    }
    // Handle float input
    else if (request->hasParam(FLOAT_INPUT)) {
      inputMessage = request->getParam(FLOAT_INPUT)->value();
      inputParam = FLOAT_INPUT;
    }
    else {
      inputMessage = "No Input Text sent on ESP32";
      inputParam = "none";
    }

    int myInt = inputMessage.toInt();
    Serial.println(inputMessage);
    if (myInt < 100) {
      Serial.println("LOW");
      delay(1000);
    }

    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // Code to define logic based on received input from HTML form
}
