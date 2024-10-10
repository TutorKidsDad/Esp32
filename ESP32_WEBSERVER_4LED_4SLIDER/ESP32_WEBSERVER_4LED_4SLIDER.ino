#include <WiFi.h>
#include <WebServer.h>

/* Put your SSID & Password */
const char* ssid = "REMOTE-CONTROL";  // Enter SSID here
const char* password = "12345678";    // Enter Password here

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

// On/Off control pins
uint8_t LED1pin = 15;
uint8_t LED2pin = 2;
uint8_t LED3pin = 4;
uint8_t LED4pin = 5;
bool LED1status = LOW;
bool LED2status = LOW;
bool LED3status = LOW;
bool LED4status = LOW;

// PWM control pins
uint8_t PWM1pin = 18;
uint8_t PWM2pin = 19;
uint8_t PWM3pin = 21;
uint8_t PWM4pin = 22;
int dutyCycle1 = 128;  // Initial duty cycle (50%)
int dutyCycle2 = 128;
int dutyCycle3 = 128;
int dutyCycle4 = 128;

const int freq = 5000;         // PWM frequency
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int pwmChannel3 = 2;
const int pwmChannel4 = 3;
const int resolution = 8;      // 8-bit resolution (values 0 - 255)

void setup() {
  Serial.begin(115200);

  // On/Off LEDs setup
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
  pinMode(LED4pin, OUTPUT);

  // PWM LEDs setup using ledcAttach (new API)
  ledcAttach(pwmChannel1, PWM1pin, resolution);
  ledcAttach(pwmChannel2, PWM2pin, resolution);
  ledcAttach(pwmChannel3, PWM3pin, resolution);
  ledcAttach(pwmChannel4, PWM4pin, resolution);

  // Set PWM frequency separately
  ledcWriteTone(pwmChannel1, freq);
  ledcWriteTone(pwmChannel2, freq);
  ledcWriteTone(pwmChannel3, freq);
  ledcWriteTone(pwmChannel4, freq);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  // Define routes for On/Off switches
  server.on("/", handle_OnConnect);
  server.on("/Switch1on", handle_Switch1on);
  server.on("/Switch1off", handle_Switch1off);
  server.on("/Switch2on", handle_Switch2on);
  server.on("/Switch2off", handle_Switch2off);
  server.on("/Switch3on", handle_Switch3on);  // Added handler
  server.on("/Switch3off", handle_Switch3off);  // Added handler
  server.on("/Switch4on", handle_Switch4on);  // Added handler
  server.on("/Switch4off", handle_Switch4off);  // Added handler

  // Define routes for PWM control
  server.on("/PWM1", handle_PWM1);
  server.on("/PWM2", handle_PWM2);
  server.on("/PWM3", handle_PWM3);  // Added handler
  server.on("/PWM4", handle_PWM4);  // Added handler

  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  // On/Off control for LEDs
  digitalWrite(LED1pin, LED1status);
  digitalWrite(LED2pin, LED2status);
  digitalWrite(LED3pin, LED3status);
  digitalWrite(LED4pin, LED4status);

  // PWM control for LEDs
  ledcWrite(pwmChannel1, dutyCycle1);
  ledcWrite(pwmChannel2, dutyCycle2);
  ledcWrite(pwmChannel3, dutyCycle3);
  ledcWrite(pwmChannel4, dutyCycle4);
}

// Handle root page
void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  dutyCycle1 = 128;
  dutyCycle2 = 128;
  dutyCycle3 = 128;
  dutyCycle4 = 128;
  Serial.println("Connected");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

// On/Off Switch Handlers
void handle_Switch1on() {
  LED1status = HIGH;
  Serial.println("GPIO15 Status: ON");
  server.send(200, "text/html", SendHTML(true, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch1off() {
  LED1status = LOW;
  Serial.println("GPIO15 Status: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch2on() {
  LED2status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, true, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch2off() {
  LED2status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch3on() {  // Added handler
  LED3status = HIGH;
  Serial.println("GPIO4 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, true, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch3off() {  // Added handler
  LED3status = LOW;
  Serial.println("GPIO4 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, false, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch4on() {  // Added handler
  LED4status = HIGH;
  Serial.println("GPIO5 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, true, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_Switch4off() {  // Added handler
  LED4status = LOW;
  Serial.println("GPIO5 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, false, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

// PWM Handlers
void handle_PWM1() {
  dutyCycle1 = server.arg("value").toInt(); // Get PWM value from URL parameter
  Serial.println("PWM1 value: " + String(dutyCycle1));
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_PWM2() {
  dutyCycle2 = server.arg("value").toInt();
  Serial.println("PWM2 value: " + String(dutyCycle2));
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_PWM3() {  // Added handler
  dutyCycle3 = server.arg("value").toInt();
  Serial.println("PWM3 value: " + String(dutyCycle3));
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_PWM4() {  // Added handler
  dutyCycle4 = server.arg("value").toInt();
  Serial.println("PWM4 value: " + String(dutyCycle4));
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4status, dutyCycle1, dutyCycle2, dutyCycle3, dutyCycle4)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat, uint8_t led2stat, uint8_t led3stat, uint8_t led4stat, int pwm1, int pwm2, int pwm3, int pwm4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-align: center; font-size: 20px; margin: 10px auto; cursor: pointer; border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += "input[type='range'] {width: 80%; height: 30px; background-color: #ddd; margin: 20px auto;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  
  ptr += "<body>\n";
  ptr += "<h1>ESP32 LED Control</h1>\n";
  
  // LED 1 Control (On/Off)
  if(led1stat) {
    ptr += "<p>LED 1 Status: ON</p><a class=\"button button-off\" href=\"/Switch1off\">Turn OFF</a>\n";
  } else {
    ptr += "<p>LED 1 Status: OFF</p><a class=\"button button-on\" href=\"/Switch1on\">Turn ON</a>\n";
  }
  
  // LED 2 Control (On/Off)
  if(led2stat) {
    ptr += "<p>LED 2 Status: ON</p><a class=\"button button-off\" href=\"/Switch2off\">Turn OFF</a>\n";
  } else {
    ptr += "<p>LED 2 Status: OFF</p><a class=\"button button-on\" href=\"/Switch2on\">Turn ON</a>\n";
  }
  
  // LED 3 Control (On/Off)
  if(led3stat) {
    ptr += "<p>LED 3 Status: ON</p><a class=\"button button-off\" href=\"/Switch3off\">Turn OFF</a>\n";
  } else {
    ptr += "<p>LED 3 Status: OFF</p><a class=\"button button-on\" href=\"/Switch3on\">Turn ON</a>\n";
  }

  // LED 4 Control (On/Off)
  if(led4stat) {
    ptr += "<p>LED 4 Status: ON</p><a class=\"button button-off\" href=\"/Switch4off\">Turn OFF</a>\n";
  } else {
    ptr += "<p>LED 4 Status: OFF</p><a class=\"button button-on\" href=\"/Switch4on\">Turn ON</a>\n";
  }

  // PWM Control for LED 1
  ptr += "<h3>LED 1 Brightness</h3>\n";
  ptr += "<input type='range' min='0' max='255' value='" + String(pwm1) + "' onchange='updatePWM1(this.value)'>\n";
  
  // PWM Control for LED 2
  ptr += "<h3>LED 2 Brightness</h3>\n";
  ptr += "<input type='range' min='0' max='255' value='" + String(pwm2) + "' onchange='updatePWM2(this.value)'>\n";
  
  // PWM Control for LED 3
  ptr += "<h3>LED 3 Brightness</h3>\n";
  ptr += "<input type='range' min='0' max='255' value='" + String(pwm3) + "' onchange='updatePWM3(this.value)'>\n";
  
  // PWM Control for LED 4
  ptr += "<h3>LED 4 Brightness</h3>\n";
  ptr += "<input type='range' min='0' max='255' value='" + String(pwm4) + "' onchange='updatePWM4(this.value)'>\n";
  
  ptr += "<script>\n";
  ptr += "function updatePWM1(val) { var xhr = new XMLHttpRequest(); xhr.open('GET', '/PWM1?value=' + val, true); xhr.send(); }\n";
  ptr += "function updatePWM2(val) { var xhr = new XMLHttpRequest(); xhr.open('GET', '/PWM2?value=' + val, true); xhr.send(); }\n";
  ptr += "function updatePWM3(val) { var xhr = new XMLHttpRequest(); xhr.open('GET', '/PWM3?value=' + val, true); xhr.send(); }\n";
  ptr += "function updatePWM4(val) { var xhr = new XMLHttpRequest(); xhr.open('GET', '/PWM4?value=' + val, true); xhr.send(); }\n";
  ptr += "</script>\n";
  
  ptr += "</body>\n";
  ptr += "</html>\n";
  
  return ptr;
}
