#include <Arduino.h>
#include <WiFiManager.h>  // Include WiFiManager
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

// Removed the direct ssid and password assignment
// const char* ssid = "";
// const char* password = "";

const char* TEXT_INPUT1 = "HTML_STR_INPUT1"; // String type input
const char* TEXT_INPUT2 = "HTML_INT_INPUT2"; // Integer type input
const char* TEXT_INPUT3 = "HTML_FLOAT_INPUT3"; // Float type input

// HTML web page to handle 3 input fields (HTML_STR_INPUT1, HTML_INT_INPUT2, HTML_FLOAT_INPUT3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 HTML Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f0f8ff;
      text-align: center;
      padding: 20px;
      margin: 0;
    }
    h1 {
      color: #0073e6;
    }
    form {
      background-color: #ffffff;
      padding: 20px;
      margin: 20px auto;
      box-shadow: 0px 0px 10px 0px rgba(0, 0, 0, 0.1);
      border-radius: 10px;
      max-width: 400px;
    }
    input[type="text"] {
      width: 80%;
      padding: 10px;
      margin: 10px 0;
      border: 1px solid #0073e6;
      border-radius: 5px;
      font-size: 16px;
    }
    input[type="submit"] {
      background-color: #0073e6;
      color: white;
      padding: 10px 20px;
      border: none;
      border-radius: 5px;
      font-size: 16px;
      cursor: pointer;
    }
    input[type="submit"]:hover {
      background-color: #005bb5;
    }
    a {
      color: #0073e6;
      text-decoration: none;
    }
    a:hover {
      text-decoration: underline;
    }
  </style>
</head>
<body>
  <h1>ESP32 Input Form</h1>
  <form action="/get">
    <label>Enter String Input:</label><br>
    <input type="text" name="HTML_STR_INPUT1" placeholder="Type a string"><br>
    <input type="submit" value="Submit">
  </form>
  <form action="/get">
    <label>Enter Integer Input:</label><br>
    <input type="text" name="HTML_INT_INPUT2" placeholder="Type an integer"><br>
    <input type="submit" value="Submit">
  </form>
  <form action="/get">
    <label>Enter Float Input:</label><br>
    <input type="text" name="HTML_FLOAT_INPUT3" placeholder="Type a float"><br>
    <input type="submit" value="Submit">
  </form>
</body>
</html>)rawliteral";

// Function to handle 404 errors
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  
  // Initialize WiFiManager
  WiFiManager wifiManager;
  
  // Automatically connect or open the configuration portal
  wifiManager.autoConnect("ESP32-Access-Point");  // Set your access point name

  // Output IP address once connected
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Serve the HTML page on the root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });

  // Handle form submissions on /get URL
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    
    // Get string input
    if (request->hasParam(TEXT_INPUT1)) {
      inputMessage = request->getParam(TEXT_INPUT1)->value();
      inputParam = TEXT_INPUT1;
    }
    // Get integer input
    else if (request->hasParam(TEXT_INPUT2)) {
      inputMessage = request->getParam(TEXT_INPUT2)->value();
      inputParam = TEXT_INPUT2;
    }
    // Get float input
    else if (request->hasParam(TEXT_INPUT3)) {
      inputMessage = request->getParam(TEXT_INPUT3)->value();
      inputParam = TEXT_INPUT3;
    }
    // Handle case where no valid input is provided
    else {
      inputMessage = "No Input Text sent on ESP32";
      inputParam = "none";
    }
    
    // Print the input message to serial
    Serial.println(inputMessage);

    // Send response back to the client
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });

  // Handle 404 errors for any undefined routes
  server.onNotFound(notFound);

  // Start the server
  server.begin();
}

void loop() {
  // No specific logic for loop here, but can be added as needed
}
