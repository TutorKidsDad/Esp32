#include <Wire.h>                // Library for I2C communication
#include "HX711.h"               // Load cell amplifier library for pressure measurement
#include <LiquidCrystal_I2C.h>    // LCD library for displaying information
#include <Arduino.h>              // Standard Arduino library
#include <WiFi.h>                 // Library for Wi-Fi communication
#include <AsyncTCP.h>             // TCP library for async web server
#include <ESPAsyncWebServer.h>    // Async Web server library for ESP

// Wi-Fi credentials
const char* ssid = "testing";     // Wi-Fi SSID
const char* password = "password"; // Wi-Fi password

// Web server parameter
const char* PARAM_INPUT_1 = "state"; // Parameter for reading state from webpage

// Timer variables
unsigned long startTime;          
unsigned long setTime = 9 * 3600 * 1000;  // 9 hours in milliseconds
bool startTimer = false;

// GPIO pins
const int upPin = 12, downPin = 14, inflatePin = 27, deflatePin = 32;
const int pumpPin = 13, valvePin = 15;
const int timerUpPin = 17, timerDownPin = 18, timerStartPin = 19, timerBuzzerPin = 16;

// Pressure sensor and control variables
HX711 scale;
double setPressure = 0, currentPressure = 0;
bool inflate = false, deflate = false, valveOpen = false;

// LCD display object (I2C address 0x3F, 16x2 display)
LiquidCrystal_I2C lcd(0x3F, 16, 2);

// Web page control variables
int valveState = LOW;
unsigned long debounceDelay = 50, lastDebounceTime = 0;
bool deflatePressed = false;

// AsyncWebServer object
AsyncWebServer server(80);

// Web page HTML
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Machine Controller</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1", true); }
  else { xhr.open("GET", "/update?state=0", true); }
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      if( this.responseText == 1){ 
        inputChecked = true;
        outputStateM = "On";
      }
      else { 
        inputChecked = false;
        outputStateM = "Off";
      }
      document.getElementById("output").checked = inputChecked;
      document.getElementById("outputState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;
</script>
</body>
</html>
)rawliteral";


// Function prototypes
void initWiFi();
void handleWebRequests();
String processor(const String& var);
String outputState();
void debounceButton(int pin, bool &state, void (*onPress)());
void controlPump();
void controlTimer();
void updateLCD();

void setup() {
  Serial.begin(115200);
  
  // Initialize HX711 and tare
  scale.begin(2, 3);    
  scale.set_scale();     
  scale.tare();          
  
  // Initialize GPIO
  pinMode(upPin, INPUT_PULLUP); pinMode(downPin, INPUT_PULLUP);
  pinMode(inflatePin, INPUT_PULLUP); pinMode(deflatePin, INPUT_PULLUP);
  pinMode(pumpPin, OUTPUT); pinMode(valvePin, OUTPUT);
  pinMode(timerBuzzerPin, OUTPUT); pinMode(timerUpPin, INPUT_PULLUP); pinMode(timerDownPin, INPUT_PULLUP); pinMode(timerStartPin, INPUT_PULLUP);
  
  digitalWrite(valvePin, LOW); digitalWrite(pumpPin, LOW);
  
  lcd.begin(16, 2); lcd.backlight();

  initWiFi();  
  handleWebRequests();
}

void loop() {
  debounceButton(upPin, inflate, []() { setPressure += 0.1; });
  debounceButton(downPin, deflate, []() { setPressure -= 0.1; });
  debounceButton(timerStartPin, startTimer, []() { startTimer = !startTimer; });

  controlPump();
  controlTimer();
  updateLCD();
}

// Initializes Wi-Fi and prints connection details
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
}

// Handles web server requests and initializes routes
void handleWebRequests() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      digitalWrite(valvePin, inputMessage.toInt());
      valveState = !valveState;
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(valvePin)).c_str());
  });

  server.begin();
}

// Processes HTML placeholders
String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String outputStateValue = outputState();
    return "<h4>Valve - GPIO 15 - State <span id=\"outputState\"></span></h4>"
           "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + ">"
           "<span class=\"slider\"></span></label>";
  }
  return String();
}

// Returns the valve state for the web interface
String outputState() {
  return digitalRead(valvePin) ? "checked" : "";
}

// Debounces button presses and triggers onPress action if state changes
void debounceButton(int pin, bool &state, void (*onPress)()) {
  int reading = digitalRead(pin);
  if (reading != state) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay && reading == LOW) {
    state = !state;
    onPress();
  }
}

// Controls the pump based on pressure readings
void controlPump() {
  currentPressure = scale.get_units();
  if (inflate && currentPressure < setPressure) {
    digitalWrite(pumpPin, HIGH);
  } else {
    digitalWrite(pumpPin, LOW);
    inflate = false;
  }
}

// Manages the timer and buzzer logic
void controlTimer() {
  if (startTimer) {
    if (millis() - startTime >= setTime) {
      digitalWrite(timerBuzzerPin, LOW);
      startTimer = false;
    } else {
      digitalWrite(timerBuzzerPin, HIGH);
    }
  }
}

// Updates the LCD with pressure and timer information
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Pressure: ");
  lcd.print(currentPressure);
  lcd.setCursor(0, 1);
  lcd.print("Set Press: ");
  lcd.print(setPressure);
}
