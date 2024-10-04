// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "value";

const int output = 2;

String timerSliderValue = "10";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// HTML content for the main page
const char index_html[] PROGMEM = R"rawliteral(
<!-- Your HTML content here -->
)rawliteral";

// Function prototypes
String processor(const String& var);
String outputState();

void setup() {
  Serial.begin(115200);

  // Set the output pin as OUTPUT and initialize it to LOW
  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);

  // Connect to Wi-Fi with timeout
  WiFi.begin(ssid, password);
  unsigned long startAttemptTime = millis();
  const unsigned long timeoutDuration = 10000;  // 10 seconds
  
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeoutDuration) {
      delay(1000);
      Serial.println("Connecting to WiFi...");
  }
  
  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to connect to WiFi");
  } else {
      Serial.println("WiFi connected.");
      Serial.println(WiFi.localIP());
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
