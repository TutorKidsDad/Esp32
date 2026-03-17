#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ROBOT";
const char* password = "12345678";

WebServer server(80);

// Motor pins
const int motor1Pin1 = 27;
const int motor1Pin2 = 26;
const int enable1Pin = 14;

const int motor2Pin1 = 33;
const int motor2Pin2 = 25;
const int enable2Pin = 32;

const int freq = 30000;
const int resolution = 8;

String webpage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Robot Joystick</title>

<style>
body { text-align:center; font-family:Arial; }
#joystick {
width:200px;
height:200px;
background:#ddd;
border-radius:50%;
margin:auto;
position:relative;
touch-action:none;
}
#stick {
width:80px;
height:80px;
background:#444;
border-radius:50%;
position:absolute;
left:60px;
top:60px;
}
</style>
</head>

<body>

<h2>Robot Joystick</h2>

<div id="joystick">
<div id="stick"></div>
</div>

<script>

let joy = document.getElementById("joystick");
let stick = document.getElementById("stick");

let centerX = 100;
let centerY = 100;

function move(e){

let rect = joy.getBoundingClientRect();

let x = e.touches[0].clientX - rect.left;
let y = e.touches[0].clientY - rect.top;

let dx = x - centerX;
let dy = y - centerY;

dx = Math.max(-80, Math.min(80, dx));
dy = Math.max(-80, Math.min(80, dy));

stick.style.left = (centerX + dx - 40) + "px";
stick.style.top = (centerY + dy - 40) + "px";

fetch(`/drive?x=${dx}&y=${dy}`);

}

function stop(){

stick.style.left = "60px";
stick.style.top = "60px";

fetch(`/drive?x=0&y=0`);

}

joy.addEventListener("touchmove", move);
joy.addEventListener("touchend", stop);

</script>

</body>
</html>
)rawliteral";


void stopMotors()
{
Serial.println("STOP");

digitalWrite(motor1Pin1,LOW);
digitalWrite(motor1Pin2,LOW);

digitalWrite(motor2Pin1,LOW);
digitalWrite(motor2Pin2,LOW);

ledcWrite(enable1Pin,0);
ledcWrite(enable2Pin,0);
}


void setup()
{

Serial.begin(115200);
Serial.println("ESP32 Robot Starting...");

pinMode(motor1Pin1,OUTPUT);
pinMode(motor1Pin2,OUTPUT);
pinMode(motor2Pin1,OUTPUT);
pinMode(motor2Pin2,OUTPUT);

ledcAttach(enable1Pin,freq,resolution);
ledcAttach(enable2Pin,freq,resolution);

WiFi.softAP(ssid,password);

Serial.println("WiFi AP Started");
Serial.print("IP Address: ");
Serial.println(WiFi.softAPIP());

server.on("/",[](){

Serial.println("Client opened webpage");
server.send(200,"text/html",webpage);

});

server.on("/drive",[](){

int x = server.arg("x").toInt();
int y = server.arg("y").toInt();

Serial.print("Joystick X: ");
Serial.print(x);
Serial.print(" Y: ");
Serial.println(y);

int speed = constrain(abs(y)*3,0,255);
int turn = constrain(abs(x)*3,0,255);

if(y < -10) // forward
{

Serial.println("FORWARD");

digitalWrite(motor1Pin1,HIGH);
digitalWrite(motor1Pin2,LOW);

digitalWrite(motor2Pin1,HIGH);
digitalWrite(motor2Pin2,LOW);

ledcWrite(enable1Pin, max(0,speed-turn));
ledcWrite(enable2Pin, speed);

}

else if(y > 10) // reverse
{

Serial.println("BACKWARD");

digitalWrite(motor1Pin1,LOW);
digitalWrite(motor1Pin2,HIGH);

digitalWrite(motor2Pin1,LOW);
digitalWrite(motor2Pin2,HIGH);

ledcWrite(enable1Pin, max(0,speed-turn));
ledcWrite(enable2Pin, speed);

}

else
{
stopMotors();
}

server.send(200,"text/plain","OK");

});

server.begin();

Serial.println("Server started");

}


void loop()
{
server.handleClient();
}
