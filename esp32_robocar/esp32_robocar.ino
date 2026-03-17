#include <WiFi.h>
#include <WebServer.h>

// Create WiFi AP
const char* ssid = "ROBOT";
const char* password = "12345678";

WebServer server(80);

// Motor A
const int motor1Pin1 = 27;
const int motor1Pin2 = 26;
const int enable1Pin = 14;

// Motor B
const int motor2Pin1 = 33;
const int motor2Pin2 = 25;
const int enable2Pin = 32;

// PWM settings
const int freq = 30000;
const int resolution = 8;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
int dutyCycle = 255;

String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>Mobile Robot</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
button{
width:100px;
height:60px;
font-size:20px;
margin:10px;
}
</style>
</head>
<body style="text-align:center">
<h2>Mobile Robot Controller</h2>

<button onclick="fetch('/forward')">Forward</button><br>
<button onclick="fetch('/left')">Left</button>
<button onclick="fetch('/stop')">Stop</button>
<button onclick="fetch('/right')">Right</button><br>
<button onclick="fetch('/backward')">Backward</button>

</body>
</html>
)rawliteral";

// Motor functions
void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void forward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void backward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void left() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void right() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void setup() {

  Serial.begin(115200);

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // PWM setup
ledcAttach(enable1Pin, freq, resolution);
ledcAttach(enable2Pin, freq, resolution);

ledcWrite(enable1Pin, dutyCycle);
ledcWrite(enable2Pin, dutyCycle);

  // Start WiFi AP
  WiFi.softAP(ssid, password);

  Serial.println("WiFi AP Started");
  Serial.println(WiFi.softAPIP());

  // Web routes
  server.on("/", []() {
    server.send(200, "text/html", webpage);
  });

  server.on("/forward", []() {
    forward();
    server.send(200, "text/plain", "Forward");
  });

  server.on("/backward", []() {
    backward();
    server.send(200, "text/plain", "Backward");
  });

  server.on("/left", []() {
    left();
    server.send(200, "text/plain", "Left");
  });

  server.on("/right", []() {
    right();
    server.send(200, "text/plain", "Right");
  });

  server.on("/stop", []() {
    stopMotors();
    server.send(200, "text/plain", "Stop");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
