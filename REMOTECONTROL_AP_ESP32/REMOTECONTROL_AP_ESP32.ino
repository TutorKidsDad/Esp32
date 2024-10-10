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

uint8_t LED1pin = 4;
uint8_t LED2pin = 5;
uint8_t LED1pwm = 0;   // PWM value for LED1 (0-255)
uint8_t LED2pwm = 0;   // PWM value for LED2 (0-255)

void setup() {
  Serial.begin(115200);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);

  // Initialize the ESP32 soft access point (AP mode)
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  // Web server routes
  server.on("/", handle_OnConnect);
  server.on("/Switch1", handle_Switch1);  // For slider control of LED1
  server.on("/Switch2", handle_Switch2);  // For slider control of LED2
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  // Set PWM values for LEDs
  analogWrite(LED1pin, LED1pwm);
  analogWrite(LED2pin, LED2pwm);
}

void handle_OnConnect() {
  Serial.println("Connection Established");
  server.send(200, "text/html", SendHTML(LED1pwm, LED2pwm));
}

void handle_Switch1() {
  if (server.hasArg("pwm")) {
    LED1pwm = server.arg("pwm").toInt();  // Get PWM value from slider
    Serial.print("LED1 PWM: ");
    Serial.println(LED1pwm);
  }
  server.send(200, "text/html", SendHTML(LED1pwm, LED2pwm));
}

void handle_Switch2() {
  if (server.hasArg("pwm")) {
    LED2pwm = server.arg("pwm").toInt();  // Get PWM value from slider
    Serial.print("LED2 PWM: ");
    Serial.println(LED2pwm);
  }
  server.send(200, "text/html", SendHTML(LED1pwm, LED2pwm));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1pwm, uint8_t led2pwm) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".slider {width: 300px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>Samvat Remote Control</h1>\n";
  ptr +="<h3>Power in your hands</h3>\n";
  
  // Slider for LED1
  ptr +="<p>LED1 Intensity</p>";
  ptr +="<input type=\"range\" class=\"slider\" id=\"led1\" min=\"0\" max=\"255\" value=\"" + String(led1pwm) + "\" onchange=\"updateSlider1(this.value)\">\n";
  
  // Slider for LED2
  ptr +="<p>LED2 Intensity</p>";
  ptr +="<input type=\"range\" class=\"slider\" id=\"led2\" min=\"0\" max=\"255\" value=\"" + String(led2pwm) + "\" onchange=\"updateSlider2(this.value)\">\n";
  
  // JavaScript to handle the slider input and send PWM values to the server
  ptr +="<script>\n";
  ptr +="function updateSlider1(val) {\n";
  ptr +="  var xhr = new XMLHttpRequest();\n";
  ptr +="  xhr.open('GET', '/Switch1?pwm=' + val, true);\n";
  ptr +="  xhr.send();\n";
  ptr +="}\n";
  
  ptr +="function updateSlider2(val) {\n";
  ptr +="  var xhr = new XMLHttpRequest();\n";
  ptr +="  xhr.open('GET', '/Switch2?pwm=' + val, true);\n";
  ptr +="  xhr.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  
  ptr +="</body>\n";
  ptr +="</html>\n";
  
  return ptr;
}
