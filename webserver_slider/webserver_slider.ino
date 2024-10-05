#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "testing";
const char* password = "password";

const int output = 2;

String sliderValue = "0";

// setting PWM properties
const int freq = 5000;
const int resolution = 8;

const char* PARAM_INPUT = "value";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Machine Controller</title>
  <style>
    /* General Styles */
    html {
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      text-align: center;
      background-color: #f4f4f9;
      color: #333;
    }
    body {
      max-width: 500px;
      margin: 0px auto;
      padding: 40px;
      background-color: #fff;
      box-shadow: 0px 0px 15px rgba(0, 0, 0, 0.1);
      border-radius: 10px;
    }
    h2 {
      font-size: 2.5rem;
      color: #003249;
      margin-bottom: 30px;
    }
    p {
      font-size: 1.5rem;
      margin: 20px 0;
    }

    /* Slider Styles */
    .slider {
      -webkit-appearance: none;
      appearance: none;
      width: 100%;
      height: 20px;
      background: #ffd65c;
      border-radius: 5px;
      outline: none;
      transition: background 0.3s ease;
      cursor: pointer;
    }
    .slider:hover {
      background: #ffc107;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 30px;
      height: 30px;
      background: #003249;
      border-radius: 50%;
      cursor: pointer;
      box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.2);
      transition: transform 0.2s ease;
    }
    .slider::-webkit-slider-thumb:hover {
      transform: scale(1.2);
    }
    .slider::-moz-range-thumb {
      width: 30px;
      height: 30px;
      background: #003249;
      border-radius: 50%;
      cursor: pointer;
      box-shadow: 0px 0px 5px rgba(0, 0, 0, 0.2);
    }

    /* Info Box */
    .info-box {
      font-size: 1.6rem;
      background-color: #e0f7fa;
      padding: 10px;
      border-radius: 5px;
      margin: 20px 0;
      color: #00796b;
    }

    /* Button Style */
    button {
      background-color: #00796b;
      color: #fff;
      border: none;
      padding: 10px 20px;
      font-size: 1rem;
      border-radius: 5px;
      cursor: pointer;
      transition: background 0.3s ease;
    }
    button:hover {
      background-color: #004d40;
    }
  </style>
</head>
<body>
  <h2>ESP Web Server</h2>

  <!-- Display Slider Value -->
  <div class="info-box">
    <p>Current Value: <span id="textSliderValue">%SLIDERVALUE%</span></p>
  </div>

  <!-- Slider Control -->
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>

  <!-- JavaScript to Handle Slider Updates -->
  <script>
    function updateSliderPWM(element) {
      var sliderValue = document.getElementById("pwmSlider").value;
      document.getElementById("textSliderValue").innerHTML = sliderValue;
      console.log(sliderValue);
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/slider?value=" + sliderValue, true);
      xhr.send();
    }
  </script>
</body>
</html>

)rawliteral";

// Replaces placeholder with slider value in your web page
String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return sliderValue;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  // configure LED PWM functionalities using the new API
  ledcAttach(output, freq, resolution);  // Attach and set up the PWM signal on GPIO 2
  
  // Initialize PWM with the default slider value
  ledcWrite(output, sliderValue.toInt());

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

  // Send a GET request to <ESP_IP>/slider?value=<inputMessage>
  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    // GET input1 value on <ESP_IP>/slider?value=<inputMessage>
    if (request->hasParam(PARAM_INPUT)) {
      inputMessage = request->getParam(PARAM_INPUT)->value();
      sliderValue = inputMessage;
      ledcWrite(output, sliderValue.toInt());  // Adjust PWM output based on slider value
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}

void loop() {
}
