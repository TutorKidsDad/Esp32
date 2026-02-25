#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

#define MQ135_PIN 34   // GPIO34 (ADC)

int moderateThreshold = 1500;
int poorThreshold = 2500;

int getAverageReading() {
  long total = 0;
  for (int i = 0; i < 10; i++) {
    total += analogRead(MQ135_PIN);
    delay(20);
  }
  return total / 10;
}

void handleRoot() {

  int airQuality = getAverageReading();
  String status;
  String color;

  if (airQuality < moderateThreshold) {
    status = "GOOD 😊";
    color = "green";
  }
  else if (airQuality >= moderateThreshold && airQuality < poorThreshold) {
    status = "MODERATE 😐";
    color = "orange";
  }
  else {
    status = "POOR 🚨";
    color = "red";
  }

  String html = "<!DOCTYPE html><html>";
  html += "<head><meta http-equiv='refresh' content='5'/>";
  html += "<title>ESP32 Air Monitor</title>";
  html += "<style>body{text-align:center;font-family:Arial;}";
  html += "h1{color:#333;} .status{color:" + color + ";font-size:30px;}";
  html += "</style></head><body>";

  html += "<h1>MQ-135 Air Quality Monitor</h1>";
  html += "<h2>Raw ADC Value: " + String(airQuality) + "</h2>";
  html += "<div class='status'>Status: " + status + "</div>";
  html += "<p>Auto refresh every 5 seconds</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {

  Serial.begin(115200);
  analogReadResolution(12);  // ESP32 ADC = 0–4095

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
