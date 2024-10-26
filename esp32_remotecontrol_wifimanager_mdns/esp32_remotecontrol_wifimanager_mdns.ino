#ifdef ESP32 // ESP32 libraries
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
  #include <WiFiManager.h> // WiFiManager library
#else // ESP8266 libraries
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266mDNS.h>
  #include <WiFiManager.h> // WiFiManager library
#endif
#include <ESPAsyncWebServer.h>

// Define Status
const char* PARAM_INPUT_1 = "state";
const char* PARAM_INPUT_2 = "state2";

// Pin Assignments
const int relay1 = 15;
const int switch1 = 12;
const int relay2 = 2;
const int switch2 = 14;

// Variables declaration
int relay_1_status = LOW;
int switch_1_status;
int last_switch_1_status = LOW;
int relay_2_status = LOW;
int switch_2_status;
int last_switch_2_status = LOW;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

AsyncWebServer server(80);

// HTML content
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>NodeMCU based Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
html {font-family: Arial; display: inline-block; text-align: center;}
h2 {font-size: 3.0rem;}
p {font-size: 3.0rem;}
body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
.switch {position: relative; display: inline-block; width: 120px; height: 68px}
.switch input {display: none}
.slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #F63E36; border-radius: 34px}
.slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
input:checked+.slider {background-color: #3CC33C}
input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
</style>
</head>
<body>
<h2>NodeMCU based Web Server</h2>
%BUTTONPLACEHOLDER%
%BUTTONPLACEHOLDER2%
<script>
function toggleCheckbox(element)
{
var xhr = new XMLHttpRequest();
if(element.checked)
{
xhr.open("GET", "/update?state=1", true);
}
else
{
xhr.open("GET", "/update?state=0", true);
}
xhr.send();
}
 
function toggleCheckbox2(element)
{
var xhr2 = new XMLHttpRequest();
if(element.checked)
{
xhr2.open("GET", "/update?state2=1", true);
}
else
{
xhr2.open("GET", "/update?state2=0", true);
}
xhr2.send();
}
 
setInterval(function ( )
{
var xhttp = new XMLHttpRequest();
xhttp.onreadystatechange = function()
{
if (this.readyState == 4 && this.status == 200)
{
var inputChecked;
var outputStateM;
 
if( this.responseText == 1)
{
inputChecked = true;
outputStateM = "ON";
}
else
{
inputChecked = false;
outputStateM = "OFF";
}
document.getElementById("output").checked = inputChecked;
document.getElementById("outputState").innerHTML = outputStateM;
}
}
xhttp.open("GET", "/state", true);
xhttp.send();
 
var xhttp2 = new XMLHttpRequest();
xhttp2.onreadystatechange = function()
{
if (this.readyState == 4 && this.status == 200)
{
var inputChecked2;
var outputStateM2;
 
if( this.responseText == 1)
{
inputChecked2 = true;
outputStateM2 = "ON";
}
else
{
inputChecked2 = false;
outputStateM2 = "OFF";
}
document.getElementById("output2").checked = inputChecked2;
document.getElementById("outputState2").innerHTML = outputStateM2;
}
};
xhttp2.open("GET", "/state2", true);
xhttp2.send();
 
}, 1000 ) ;
</script>
</body>
</html>
)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var) {
  if(var == "BUTTONPLACEHOLDER") {
    String buttons1 = "";
    String outputStateValue = outputState();
    buttons1 += "<h4>Device 1 - Status <span id=\"outputState\"><span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons1;
  }
  if(var == "BUTTONPLACEHOLDER2") {
    String buttons2 = "";
    String outputStateValue2 = outputState2();
    buttons2 += "<h4>Device 2 - Status <span id=\"outputState2\"><span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox2(this)\" id=\"output2\" " + outputStateValue2 + "><span class=\"slider\"></span></label>";
    return buttons2;
  }
  return String();
}

String outputState() {
  return digitalRead(relay1) ? "checked" : "";
}

String outputState2() {
  return digitalRead(relay2) ? "checked" : "";
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // Set relay and switch pins
  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, LOW);
  pinMode(switch1, INPUT);
  
  pinMode(relay2, OUTPUT);
  digitalWrite(relay2, LOW);
  pinMode(switch2, INPUT);

  // WiFiManager instance
  WiFiManager wm;
  if (!wm.autoConnect("AutoConnectAP")) {
    Serial.println("Failed to connect or hit timeout");
    ESP.restart();
  }
  Serial.println("Connected to Wi-Fi");

  // mDNS setup
  if (!MDNS.begin("sensor1")) { // Replace "sensor1" with your desired hostname
    Serial.println("Error starting mDNS");
    return;
  }
  Serial.println("mDNS responder started");

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Routes for web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });
  
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    String inputMessage;
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      digitalWrite(relay1, inputMessage.toInt());
      relay_1_status = !relay_1_status;
    }
    request->send(200, "text/plain", "OK");

    String inputMessage2;
    if (request->hasParam(PARAM_INPUT_2)) {
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(relay2, inputMessage2.toInt());
      relay_2_status = !relay_2_status;
    }
    request->send(200, "text/plain", "OK");
  });

  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(relay1)).c_str());
  });

  server.on("/state2", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(relay2)).c_str());
  });

  server.begin();
}

void loop() {
  int reading1 = digitalRead(switch1);
  if (reading1 != last_switch_1_status) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) { 
    if (reading1 != switch_1_status) {
      switch_1_status = reading1;
      if (switch_1_status == HIGH) {
        relay_1_status = !relay_1_status;
      }
    }
  }

  int reading2 = digitalRead(switch2);
  if (reading2 != last_switch_2_status) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading2 != switch_2_status) {
      switch_2_status = reading2;
      if (switch_2_status == HIGH) {
        relay_2_status = !relay_2_status;
      }
    }
  }
  
  digitalWrite(relay1, relay_1_status);
  digitalWrite(relay2, relay_2_status);

  last_switch_1_status = reading1;
  last_switch_2_status = reading2;
}
