#include <WiFi.h>
#include <WebServer.h>
#include <WiFiManager.h> // Include WiFiManager library
#include <ESPmDNS.h>     // Include mDNS library
#include "NewPing.h"

// WiFiManager AP Settings
WiFiManager wm;  // Create a WiFiManager instance

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

const int trigPin = 5;
const int echoPin = 18;

// Define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 200

// NewPing setup of pins and maximum distance.
NewPing sonar(trigPin, echoPin, MAX_DISTANCE);

long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  // Attempt to connect to Wi-Fi or open configuration portal
  if (!wm.autoConnect("AutoConnectAP", "password")) {  // Name and password of the portal
    Serial.println("Failed to connect and hit timeout");
    ESP.restart(); // Restart on connection failure
  }

  Serial.println("WiFi connected successfully!");

  // Configure static IP after successful connection
  WiFi.config(local_ip, gateway, subnet);

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Start mDNS service with a custom hostname
  if (!MDNS.begin("myesp")) {  // Change "myesp" to the desired hostname
    Serial.println("Error starting mDNS");
    return;
  }
  Serial.println("mDNS responder started. Access at http://myesp.local");

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

void handle_OnConnect() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = 200 - sonar.ping_cm();

  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;

  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  delay(1000);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
  delay(1000);

  delay(1000);
  server.send(200, "text/html", SendHTML(distanceCm, distanceInch));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float distanceCm, float distanceInch) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<link href=\"https://fonts.googleapis.com/css?family=Open+Sans:300,400,600\" rel=\"stylesheet\">\n";
  ptr += "<title>Smart Height Measurement</title>\n";
  ptr += "<style>\n";
  ptr += "html { font-family: 'Open Sans', sans-serif; display: block; margin: 0 auto; text-align: center; color: #333333;}\n";
  ptr += "body { margin-top: 50px; background-color: #f4f4f4; padding: 20px; border-radius: 10px; box-shadow: 0 0 10px rgba(0,0,0,0.1);}\n";
  ptr += "h1 { margin: 50px auto 30px; font-size: 2.5em; color: #3498db;}\n";
  ptr += ".data { display: flex; justify-content: space-around; align-items: center; margin: 20px 0; padding: 20px; background: white; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.2);}\n";
  ptr += ".icon { width: 40px; height: 40px; }\n";
  ptr += ".distance-text { font-weight: 600; font-size: 24px; text-align: center; }\n";
  ptr += ".distance-value { font-weight: 300; font-size: 48px; color: #3498db; margin-top: 10px; }\n";
  ptr += ".subscript { font-size: 18px; font-weight: 400; }\n";
  ptr += ".divider { border-left: 2px solid #3498db; height: 80px; margin: 0 20px; }\n";
  ptr += "</style>\n";
  ptr += "<script>\n";
  ptr += "setInterval(function() { window.location.reload(); }, 5000);\n";
  ptr += "</script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";

  ptr += "<h1>Smart Height Measurement</h1>\n";
  ptr += "<div class=\"data\">\n";

  // Distance in cm
  ptr += "<div>\n";
  ptr += "<img src=\"data:image/svg+xml;base64,...\" class=\"icon\" alt=\"Distance in cm\">\n"; // Replace with actual icon data
  ptr += "<div class=\"distance-text\">Distance (cm)</div>\n";
  ptr += "<div class=\"distance-value\">" + String((int)distanceCm) + "<span class=\"subscript\"> cm</span></div>\n";
  ptr += "</div>\n";

  // Divider
  ptr += "<div class=\"divider\"></div>\n";

  // Distance in inches
  ptr += "<div>\n";
  ptr += "<img src=\"data:image/svg+xml;base64,...\" class=\"icon\" alt=\"Distance in inch\">\n"; // Replace with actual icon data
  ptr += "<div class=\"distance-text\">Distance (in)</div>\n";
  ptr += "<div class=\"distance-value\">" + String((int)distanceInch) + "<span class=\"subscript\"> inch</span></div>\n";
  ptr += "</div>\n";

  ptr += "</div>\n"; // Close data div
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
