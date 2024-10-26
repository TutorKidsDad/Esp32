// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
  #include <WiFiManager.h>  // Include the WiFiManager library
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "value";

const int output = 2;

String timerSliderValue = "10";

// HTML content for the main page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32 Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    /* Add your styles here */
  </style>
  <script>
    function toggleCheckbox(checkbox) {
      var xhr = new XMLHttpRequest();
      var state = checkbox.checked ? 1 : 0;
      xhr.open("GET", "/update?state=" + state, true);
      xhr.send();
    }

    function updateSlider(value) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider?value=" + value, true);
      xhr.send();
    }
  </script>
</head>
<body>
  <h1>ESP32 Control</h1>
  <div id="buttons">
    BUTTONPLACEHOLDER
  </div>
  <input type="range" min="1" max="60" value="TIMERVALUE" oninput="updateSlider(this.value)">
</body>
</html>
)rawliteral";

// Function prototypes
String processor(const String& var);
String outputState();

void setup() {
  Serial.begin(115200);

  // Set the output pin as OUTPUT and initialize it to LOW
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);

  // Initialize WiFiManager
  WiFiManager wifiManager;

  // Automatically connect to Wi-Fi
  if (!wifiManager.autoConnect("ESP32-AP")) {
    Serial.println("Failed to connect. Restarting...");
    delay(3000);
    ESP.restart();  // Restart if connection fails
  } else {
    Serial.println("WiFi connected.");
    Serial.println(WiFi.localIP());
  }

  // Start mDNS service
  if (MDNS.begin("esp32")) {
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error starting mDNS");
  }

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Route to handle output state changes
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      int outputState = inputMessage.toInt();
      if (outputState == 0 || outputState == 1) {
        digitalWrite(output, outputState);
        Serial.print("Output state changed to: ");
        Serial.println(outputState);
      } else {
        Serial.println("Invalid output state");
      }
    } else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Route to handle slider value changes
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      timerSliderValue = inputMessage;
    } else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Start the server
  server.begin();
}

void loop() {
  // Your main loop code (if any)
}

String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    String outputStateValue = outputState();
    buttons+= "<p><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label></p>";
    return buttons;
  }
  else if(var == "TIMERVALUE"){
    return timerSliderValue;
  }
  return String();
}

String outputState(){
  return digitalRead(output) ? "checked" : "";
}
