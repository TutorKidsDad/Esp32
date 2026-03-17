
//for esp32
//#include <WiFi.h>
//#include <WebServer.h>
//WebServer server(80);


//for esp8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);



const char* ssid = "Orion_Robot";
const char* password = "12345678";

/*

// Motor pins for M3 & M4
#define IN1 14
#define IN2 12
#define IN3 13
#define IN4 15
*/
// Motor pins for M1 & M2 (L293D shield)

#define IN1 5   // D1
#define IN2 4   // D2
#define IN3 14  // D5
#define IN4 12  // D6

void setup() {

  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  WiFi.softAP(ssid, password);

  Serial.println("WiFi Started");
  Serial.print("Connect to IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/cmd", handleCommand);

  server.begin();
}

void loop() {
  server.handleClient();
}

void handleCommand() {

  String cmd = server.arg("go");

  if(cmd == "F") {
    Serial.println("Forward command received");
    forward();
  }

  else if(cmd == "B") {
    Serial.println("Backward command received");
    backward();
  }

  else if(cmd == "L") {
    Serial.println("Left command received");
    left();
  }

  else if(cmd == "R") {
    Serial.println("Right command received");
    right();
  }

  else if(cmd == "S") {
    Serial.println("Stop command received");
    stopCar();
  }

  server.send(200,"text/plain","OK");
}

void forward(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void backward(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void left(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

void right(){
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
}

void stopCar(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
}

void handleRoot(){

String page = R"=====(

<!DOCTYPE html>
<html>
<head>

<meta name="viewport" content="width=device-width, initial-scale=1">

<style>

body{
font-family:Arial;
text-align:center;
background:#111;
color:white;
}

button{
width:90px;
height:90px;
font-size:35px;
margin:10px;
border-radius:15px;
border:none;
background:#00aaff;
color:white;
}

button:active{
background:#ff6600;
}

</style>

<script>

function send(cmd){
fetch("/cmd?go="+cmd);
}

</script>

</head>

<body>

<h2>ESP32 Robot</h2>

<div>
<button ontouchstart="send('F')" ontouchend="send('S')">⬆</button>
</div>

<div>

<button ontouchstart="send('L')" ontouchend="send('S')">⬅</button>

<button onclick="send('S')">⏹</button>

<button ontouchstart="send('R')" ontouchend="send('S')">➡</button>

</div>

<div>
<button ontouchstart="send('B')" ontouchend="send('S')">⬇</button>
</div>

</body>
</html>

)=====";

server.send(200,"text/html",page);

}
