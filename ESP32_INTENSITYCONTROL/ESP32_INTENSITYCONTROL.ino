/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-server-slider-pwm/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
*********/

// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";

const int output = 2;
String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Optimized HTML code with improved aesthetics
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP Web Server</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 0;
      background-color: #f0f0f0;
    }
    .container {
      max-width: 600px;
      margin: 50px auto;
      padding: 20px;
      background-color: #fff;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      border-radius: 8px;
    }
    h2 {
      color: #003249;
      margin-bottom: 20px;
    }
    p {
      font-size: 1.5rem;
      color: #333;
    }
    .slider-container {
      margin: 20px;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 15px;
      border-radius: 5px;
      background: #FFD65C;
      outline: none;
      transition: opacity .2s;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      background: #003249;
      cursor: pointer;
      border-radius: 50%;
    }
    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      background: #003249;
      cursor: pointer;
      border-radius: 50%;
    }
    .value-display {
      font-size: 1.8rem;
      color: #003249;
      margin-top: 20px;
    }
    .footer {
      margin-top: 20px;
      font-size: 0.9rem;
      color: #666;
    }
  </style>
</head>
<body>
  <div class="container">
    <h2>ESP32 PWM Slider Control</h2>
    <p>Adjust the slider to change the PWM value</p>
    <div class="slider-container">
      <input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" class="slider">
    </div>
    <p class="value-display">PWM Value: <span id="textSliderValue">%SLIDERVALUE%</span></p>
  </div>

  <div class="footer">
    <p>ESP32 Web Server PWM Control</p>
  </div>

  <script>
    function updateSliderPWM(element) {
      var sliderValue = document.getElementById("pwmSlider").value;
      document.getElementById("textSliderValue").innerHTML = sliderValue;
      console.log(sliderValue);
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider?value="+sliderValue, true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

// Replaces placeholder with slider value in HTML
String processor(const String& var){
  if (var == "SLIDERVALUE") {
    return sliderValue;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Configure LED PWM functionalities
  ledcAttach(ledChannel, freq, resolution);
 // ledcAttachPin(output, ledChannel);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle slider value input
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if (request->hasParam(PARAM_INPUT)) {
      sliderValue = request->getParam(PARAM_INPUT)->value();
      ledcWrite(ledChannel, sliderValue.toInt());
      Serial.println("PWM Value: " + sliderValue);
    }
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();
}

void loop(){
  // Nothing to do here for this simple web server
}
