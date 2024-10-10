#include <Wire.h>
#include <TaskScheduler.h>
#include <WiFi.h>
#include <WebServer.h>

// SHT31 I2C address
#define Addr 0x44

// Static IP for AP
IPAddress ap_local_IP(192, 168, 1, 4);
IPAddress ap_gateway(192, 168, 1, 254);
IPAddress ap_subnet(255, 255, 255, 0);

// SSID and AP for local WiFi in STA mode
const char WiFissid[] = "Accesspoint";
const char WiFipass[] = "password";
// SSID and pass for AP
const char APssid[] = "Accesspoint1";
const char APpass[] = "password";

// Thingspeak host ID and API key 
const char* hostId = "Your_Things_speak_hostid";
String apiKey = "Things_speak_api_key";

volatile float humid; 
volatile float tempC;
volatile float tempF;

// Prototype for task callback
void taskI2CCallback();
void taskAPCallback();
void taskWiFiCallback();

// Webpage 1
String PAGE1 = 
"<!DOCTYPE html>"
"<html>"
"<head>"
"<meta charset=\"utf-8\">"
"<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0\">"
"<title> My first HTML page</title>"
"</head>"
"<body>"
"<h1 style= \"color:blue\"> ESP8266 Controlling temperature and Humidity</h1>"
"<p>This web application is deployed on ESP32. It shows real-time temperature and humidity. You can also see analytics on Things Speak.</p>"
"<button>"
"<a href=\"/Value\">Page2</a>"
"</button>"
"</body>"
"</html>";

WebServer server(80);
Scheduler ts;

// Tasks for I2C, hosting web server and posting on Thingspeak
Task tI2C(1 * TASK_SECOND, TASK_FOREVER, &taskI2CCallback, &ts, false);
Task tAP(5 * TASK_SECOND, TASK_FOREVER, &taskAPCallback, &ts, false);
Task tWiFi(5 * TASK_SECOND, TASK_FOREVER, &taskWiFiCallback, &ts, false);

unsigned long lastI2CMillis = 0;
unsigned long lastAPMillis = 0;
unsigned long lastWiFiMillis = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Wire.begin(21, 22); // ESP32 default I2C pins

  server.on("/", onHandleDataRoot);
  server.on("/Value", onHandleDataFeed);
  server.onNotFound(onHandleNotFound);

  // Enable I2C task
  tI2C.enable();
}

void loop() {
  // Execute the scheduler
  ts.execute(); 
}

// I2C task callback
void taskI2CCallback() {
  if (millis() - lastI2CMillis > 10000) { // 10 seconds timeout
    Serial.println("I2C task timed out.");
    tI2C.disable(); // Disable the I2C task
    reconnectAPWiFi(); // Switch to AP mode
    tAP.enable(); // Enable AP task
    return;
  }

  // I2C reading code here...
  Serial.println("I2C task started");
  unsigned int root[6];

  // Begin transmission
  Wire.beginTransmission(Addr);
  Wire.write(0x2C); // Trigger measurement
  Wire.write(0x06);
  Wire.endTransmission();

  Wire.requestFrom(Addr, 6);
  if (Wire.available() == 6) {
    root[0] = Wire.read();
    root[1] = Wire.read();
    root[2] = Wire.read(); // CRC
    root[3] = Wire.read();
    root[4] = Wire.read(); // CRC
    root[5] = Wire.read(); // CRC
  }
  
  // Process temperature and humidity
  int temp = (root[0] * 256) + root[1];
  float cTemp = -45.0 + (175.0 * temp / 65535.0);
  float humidity = (100.0 * ((root[3] * 256.0) + root[4])) / 65535.0;

  tempC = cTemp;
  humid = humidity;
  
  Serial.print("Temperature in C: ");
  Serial.println(tempC);
  Serial.print("Humidity: ");
  Serial.println(humid);

  lastI2CMillis = millis(); // Reset timer
}

// Callback for AP task
void taskAPCallback() {
  if (millis() - lastAPMillis > 10000) { // 10 seconds timeout
    Serial.println("AP task timed out.");
    tAP.disable(); // Disable AP task
    reconnectWiFi(); // Switch to STA mode
    tWiFi.enable(); // Enable WiFi task
    return;
  }

  server.handleClient(); // Handle incoming requests
  lastAPMillis = millis(); // Reset timer
}

// Callback for WiFi task
void taskWiFiCallback() {
  if (WiFi.status() == WL_CONNECTED) {
    // Send data to Thingspeak
    WiFiClient wifiClient;
    if (wifiClient.connect(hostId, 80)) {
      String postStr = apiKey + "&field1=" + String(humid) + "&field2=" + String(tempC) + "\r\n\r\n";
      wifiClient.print("POST /update HTTP/1.1\n");
      wifiClient.print("Host: api.thingspeak.com\n");
      wifiClient.print("Connection: close\n");
      wifiClient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
      wifiClient.print("Content-Type: application/x-www-form-urlencoded\n");
      wifiClient.print("Content-Length: ");
      wifiClient.print(postStr.length());
      wifiClient.print("\n\n");
      wifiClient.print(postStr);
    }
    wifiClient.stop();
  }
}

// Remaining function implementations unchanged...

// Handle the root page
void onHandleDataRoot() {
  server.send(200, "text/html", PAGE1);
}

// Handle the data feed page
void onHandleDataFeed() {
  String PAGE2 = 
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<meta charset=\"utf-8\">"
  "<meta name = \"viewport\" content = \"width = device-width, initial-scale = 1.0\">"
  "<title>Temperature and Humidity</title>"
  "</head>"
  "<body>"
  "<h1 style= \"color:blue\">Temperature and Humidity</h1>"
  "<h3 style= \"font-family:verdana; color:blue\">Temperature in Celcius:</h3>"
  "<p style=\"font-size:160%; font-family:Lucida Console\">" + String(tempC, 1) + "</p>"
  "<h3 style= \"font-family:verdana; color:blue\">Humidity:</h3>"
  "<p style=\"font-size:160%; font-family:Lucida Console\">" + String(humid, 1) + "</p>"
  "</body>"
  "</html>";

  server.send(200, "text/html", PAGE2);
}

// Handle 404 errors
void onHandleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(404, "text/plain", message);
}

// AP WiFi connection
void reconnectAPWiFi() {
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  WiFi.disconnect();
  boolean status = WiFi.softAPConfig(ap_local_IP, ap_gateway, ap_subnet);
  if (status) {
    Serial.print("Setting soft-AP ... ");
    boolean ap = WiFi.softAP(APssid, APpass);
    if (ap) {
      Serial.print("Connected to:\t");
      Serial.println(WiFi.softAPIP());
    }
    server.begin();
  }
}

// WiFi connection
void reconnectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WiFissid, WiFipass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected to:\t");
  Serial.println(WiFi.localIP());
}
