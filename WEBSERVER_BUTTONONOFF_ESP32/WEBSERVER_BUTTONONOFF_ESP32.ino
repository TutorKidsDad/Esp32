/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-physical-button/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

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
const char* ssid = "testing";
const char* password = "password";

const char* PARAM_INPUT_1 = "state";

//const int output = 2;
//const int buttonPin = 4;

const int buttonPin = 32;//deflate
const int valvePin = 15;
const int pumpPin = 13;

// Variables will change:
int ledState = LOW;          // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    /* General Styles */
    html {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      text-align: center;
      background-color: #f4f4f9;
      color: #333;
    }
    body {
      max-width: 600px;
      margin: 0px auto;
      padding-bottom: 25px;
      background-color: #fff;
      box-shadow: 0px 0px 15px rgba(0, 0, 0, 0.1);
      padding: 40px;
      border-radius: 10px;
    }
    h2 {
      font-size: 3.0rem;
      color: #2196F3;
      margin-bottom: 40px;
    }
    p {
      font-size: 2.0rem;
      color: #333;
    }

    /* Switch Styles */
    .switch {
      position: relative;
      display: inline-block;
      width: 120px;
      height: 68px;
    }
    .switch input {
      display: none;
    }
    .slider {
      position: absolute;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      border-radius: 34px;
      transition: background-color 0.4s ease;
    }
    .slider:before {
      position: absolute;
      content: "";
      height: 52px;
      width: 52px;
      left: 8px;
      bottom: 8px;
      background-color: white;
      transition: transform 0.4s ease, background-color 0.4s ease;
      border-radius: 50%;
      box-shadow: 0px 0px 10px rgba(0, 0, 0, 0.2);
    }
    input:checked + .slider {
      background-color: #2196F3;
    }
    input:checked + .slider:before {
      transform: translateX(52px);
    }

    /* Button Placeholder Style */
    #outputState {
      font-size: 2.0rem;
      font-weight: bold;
      margin-top: 20px;
      display: block;
      color: #ff5722;
    }
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>

  <!-- Switch with Output State -->
  <label class="switch">
    <input type="checkbox" id="output" onclick="toggleCheckbox(this)">
    <span class="slider"></span>
  </label>

  <!-- Output State Text -->
  <p>Output is: <span id="outputState">Off</span></p>

  <script>
    function toggleCheckbox(element) {
      var xhr = new XMLHttpRequest();
      if(element.checked){ 
        xhr.open("GET", "/update?state=1", true); 
      } else { 
        xhr.open("GET", "/update?state=0", true); 
      }
      xhr.send();
    }

    // Periodically check the state and update the switch and output text
    setInterval(function () {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var inputChecked;
          var outputStateM;
          if (this.responseText == "1") { 
            inputChecked = true;
            outputStateM = "On";
          } else { 
            inputChecked = false;
            outputStateM = "Off";
          }
          document.getElementById("output").checked = inputChecked;
          document.getElementById("outputState").innerHTML = outputStateM;
        }
      };
      xhttp.open("GET", "/state", true);
      xhttp.send();
    }, 1000);
  </script>
</body>
</html>

)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String outputStateValue = outputState();
    buttons+= "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(){
  if(digitalRead(valvePin)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(valvePin, OUTPUT);
  digitalWrite(valvePin, LOW);
  pinMode(buttonPin, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(valvePin, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(valvePin)).c_str());
  });
  // Start server
  server.begin();
}
  
void loop() {
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(valvePin, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
