#include <Wire.h>
#include "HX711.h"
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
 #include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// Replace with your network credentials
const char* ssid = "testing";
const char* password = "password";

const char* PARAM_INPUT_1 = "state";
unsigned long startTime;
unsigned long setTime;
bool startTimer = false;
////////define pins
const int upPin = 12;
const int downPin = 14;
const int inflatePin = 27;
const int deflatepin = 32;
const int pumpPin = 13;
const int valvePin = 15;

const int timerUpPin = 17;
const int timerDownPin = 18;
const int timerStartPin = 19;
const int timerBuzzerPin = 16;

HX711 scale;
double setPressure = 0;
double currentPressure = 0;
double prevPressure = 0;
bool inflate = false;
bool deflate = false;
bool valveOpen = false;
unsigned long lastPressTime = 0;

LiquidCrystal_I2C lcd(0x3F,16,2);
//LiquidCrystal_I2C lcd(0x27, 16, 2);
////////////////////////////////////////webpage start 
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
////////////////////////////////////////////////

void setup() {
  Serial.begin(115200);
  scale.begin(2, 3);
  scale.set_scale();
  scale.tare();

  pinMode(upPin, INPUT_PULLUP);//UP
  pinMode(downPin, INPUT_PULLUP);//DOWN
  pinMode(inflatePin, INPUT_PULLUP);//INFLATE
  pinMode(deflatepin, INPUT_PULLUP);//DEFLATE
  pinMode(pumpPin, OUTPUT);//PUMP
  pinMode(valvePin, OUTPUT);//VALVE
  digitalWrite(valvePin, LOW);
  digitalWrite(pumpPin, LOW);
   lcd.begin(16,2);
  lcd.backlight();
  //TIMER 
  pinMode(timerBuzzerPin, OUTPUT);//TIMER BUZZER
  pinMode(timerUpPin, INPUT_PULLUP);//TIMER UP
  pinMode(timerDownPin, INPUT_PULLUP);//TIMER DOWN
  pinMode(timerStartPin, INPUT_PULLUP);//TIMER START
  setTime = 9 * 3600 * 1000;  // 9 hours in milliseconds
  //////////////////////webpage initialize
   // Connect to Wi-Fi
WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
   // return;
  }
  Serial.println();
  Serial.println("WiFi function starting...");
  Serial.println("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
delay(5000);

  

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
  /////////////////////////////////////////
}

void loop() {
  ///////////////////////////////////webpage control
  // read the state of the switch into a local variable:
  int reading = digitalRead(deflatepin);

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
  /////////////////////////////////////////////////////////////////////
    // Check if set time button is pressed
  if (!digitalRead(timerUpPin)) {
    setTime += 1000;  // Increase set time by 1 second
  }
  if (!digitalRead(timerDownPin)) {
    setTime -= 1000;  // Decrease set time by 1 second
  }
  
  // Check if start timer button is pressed
  if (!digitalRead(timerStartPin) && !startTimer) {
    startTimer = true;
    startTime = millis();
  }
  
  // Check if stop timer button is pressed
  if (!digitalRead(timerStartPin) && startTimer) {
    startTimer = false;
  }
  
  // If timer is started, turn on LED and check if time is up
  if (startTimer) {
    digitalWrite(timerBuzzerPin, HIGH);
    if (millis() - startTime >= setTime) {
      startTimer = false;
      digitalWrite(timerBuzzerPin, LOW);
    }
  }
  // Check if set pressure buttons are pressed
  if (!digitalRead(upPin)) {
    setPressure += 0.1;  // Increase set pressure by 0.1 units
  }
  if (!digitalRead(downPin)) {
    setPressure -= 0.1;  // Decrease set pressure by 0.1 units
  }
  
  // Check if inflate button is pressed
  if (!digitalRead(inflatePin) && !inflate) {
    inflate = true;
    digitalWrite(pumpPin, HIGH);  // Turn on pump
  }
  
  // Check if deflate button is pressed twice
 if (digitalRead(deflatepin) == LOW) {
    unsigned long currentTime = millis();
    if (!deflate) {
      lastPressTime = currentTime;
      deflate = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Press once more");
    } else if (currentTime - lastPressTime >= 500) {
      deflate = false;
      if (!valveOpen) {
        digitalWrite(valvePin, HIGH);
        valveOpen = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Valve opened");
      } else {
        digitalWrite(valvePin, LOW);
        valveOpen = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Valve closed");
      }
    }
  } else {
    deflate = false;
  }
  delay(10);
  
  // Read current pressure
  currentPressure = scale.get_units();
  
  // If pump is on, turn off pump when set pressure is reached
  if (inflate) {
    if (currentPressure >= setPressure) {
      inflate = false;
      digitalWrite(pumpPin, LOW);  // Turn off pump
    } else {
      // Check if pressure is not increasing
      if (currentPressure <= prevPressure) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("WARNING: CHECK CUF");  // Display warning message
        lcd.setCursor(0, 1);
        lcd.print("Pressure not increasing");
      }
    }
  }
  
  // If solenoid valve is on, turn off solenoid valve when set pressure is reached
  if (deflate) {
    if (currentPressure <= setPressure) {
      deflate = false;
      digitalWrite(valvePin, LOW);  // Turn off solenoid valve
    }
  }
  
  // Display set pressure and current pressure on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Pressure: " + String(setPressure));
 Serial.println("Set Pressure: " + String(setPressure));
  delay(100);
  lcd.setCursor(0, 1);
  lcd.print("Current Pressure: " + String(currentPressure));
 Serial.println("Current Pressure: " + String(currentPressure));
  delay(100);
  
  prevPressure = currentPressure;
  
  delay(100);
}
