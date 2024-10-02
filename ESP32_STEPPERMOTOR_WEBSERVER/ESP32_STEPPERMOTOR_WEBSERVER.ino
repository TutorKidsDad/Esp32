

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Stepper.h>

// Stepper Motor Settings
const int stepsPerRevolution = 2048;  

#define IN1 19
#define IN2 18
#define IN3 5
#define IN4 17
#define IN5 16
#define IN6 4
#define IN7 2
#define IN8 15

Stepper stepper1(stepsPerRevolution, IN1, IN3, IN2, IN4);
Stepper stepper2(stepsPerRevolution, IN5, IN6, IN7, IN8);

// DC Motor Settings
int motorPin1 = 27;
int motorPin2 = 26;
int enablePin = 14;

const int pwmFreq = 30000;
const int pwmChannel = 0;
const int pwmResolution = 8;
int dutyCycle = 200;

// WiFi Credentials
const char* ssid = "";
const char* password = "";

// Web Server
AsyncWebServer server(80);

// HTML Form Parameters
const char* PARAM_DIR1 = "direction1";
const char* PARAM_STEPS1 = "steps1";
const char* PARAM_DIR2 = "direction2";
const char* PARAM_STEPS2 = "steps2";

// Request flags
String direction1 = "CW";
String steps1 = "0";
String direction2 = "CW";
String steps2 = "0";
bool newRequest = false;

// HTML for Web Interface
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Stepper Motor Control</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; }
    form { margin: 20px; padding: 10px; border: 1px solid #ddd; display: inline-block; }
    h1 { color: #333; }
    input[type="submit"] { padding: 10px 20px; margin-top: 10px; }
  </style>
</head>
<body>
  <h1>Stepper Motor Control</h1>
  <h2>Motor 1</h2>
  <form action="/" method="POST">
    <input type="radio" name="direction1" value="CW" checked> Clockwise
    <input type="radio" name="direction1" value="CCW"> Counterclockwise <br><br>
    <label>Steps:</label>
    <input type="number" name="steps1" min="0" value="0"><br><br>
    <input type="submit" value="Move Motor 1">
  </form>

  <h2>Motor 2</h2>
  <form action="/" method="POST">
    <input type="radio" name="direction2" value="CW" checked> Clockwise
    <input type="radio" name="direction2" value="CCW"> Counterclockwise <br><br>
    <label>Steps:</label>
    <input type="number" name="steps2" min="0" value="0"><br><br>
    <input type="submit" value="Move Motor 2">
  </form>
</body>
</html>
)rawliteral";

// Initialize WiFi connection
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();

  // Setup DC motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  ledcAttach(pwmChannel, pwmFreq, pwmResolution);
 // ledcAttachPin(enablePin, pwmChannel);

  // Setup stepper motors
  stepper1.setSpeed(5);
  stepper2.setSpeed(5);

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", index_html);
  });

  // Handle POST requests
  server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
    int params = request->params();
    for (int i = 0; i < params; i++) {
      AsyncWebParameter* p = request->getParam(i);
      if (p->isPost()) {
        if (p->name() == PARAM_DIR1) direction1 = p->value();
        if (p->name() == PARAM_STEPS1) steps1 = p->value();
        if (p->name() == PARAM_DIR2) direction2 = p->value();
        if (p->name() == PARAM_STEPS2) steps2 = p->value();
      }
    }
    request->send(200, "text/html", index_html);
    newRequest = true;
  });

  server.begin();
}

void loop() {
  // Control DC Motor
  digitalWrite(enablePin, HIGH);
  delay(100);
  
  // Move DC motor forward and backward
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  delay(2000);
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  delay(1000);
  
  // Move stepper motors based on user input
  if (newRequest) {
    if (direction1 == "CW") stepper1.step(steps1.toInt());
    else stepper1.step(-steps1.toInt());

    if (direction2 == "CW") stepper2.step(steps2.toInt());
    else stepper2.step(-steps2.toInt());

    newRequest = false;
  }

  // Gradually increase DC motor speed
  for (int i = 200; i <= 255; i += 5) {
    ledcWrite(pwmChannel, i);
    delay(500);
  }

  digitalWrite(enablePin, LOW);
}
