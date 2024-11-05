#include <WiFiManager.h>  // Include the WiFiManager library
#include <ESPmDNS.h>      // Include the mDNS library

// LED pin definition
const int ledPin = 2;  // Change to your desired LED pin

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Set up LED pin as output and turn it off initially
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Create an instance of WiFiManager
  WiFiManager wifiManager;

  // AutoConnect will attempt to connect to the last known network, or start an AP + Web portal for configuration
  if (!wifiManager.autoConnect("LED_Controller_AP")) {
    Serial.println("Failed to connect and hit timeout");
    ESP.restart();
  }

  // Print the IP address once connected
  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start mDNS responder with the hostname "ledcontrol"
  if (!MDNS.begin("ledcontrol")) {
    Serial.println("Error starting mDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started. Access at http://ledcontrol.local");

  // Add mDNS service for HTTP
  MDNS.addService("http", "tcp", 80);
}

void loop() {
  // Create an HTTP server on port 80
  WiFiClient client = WiFiServer(80).available();

  if (client) {
    Serial.println("New Client Connected.");
    String currentLine = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);  // Debugging output in the serial monitor

        // Check if the HTTP request ends
        if (c == '\n') {
          // If current line is blank, then the HTTP request has ended
          if (currentLine.length() == 0) {
            // Send the HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print("<!DOCTYPE html><html><head><title>ESP32 LED Control</title></head><body>");
            client.print("<h1>Control the LED</h1>");
            client.print("<p>LED State: ");
            client.print((digitalRead(ledPin) == HIGH) ? "ON" : "OFF");
            client.print("</p>");
            client.print("<button onclick=\"toggleLED()\">Toggle LED</button>");
            client.print("<script>function toggleLED() {var xhr = new XMLHttpRequest(); xhr.open('GET', '/toggle', true); xhr.send();}</script>");
            client.print("</body></html>");
            client.println();
            break;  // Break the while loop
          } else {
            // Reset current line
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;  // Add each character to the current line
        }

        // Check for the toggle LED request
        if (currentLine.endsWith("GET /toggle")) {
          digitalWrite(ledPin, !digitalRead(ledPin));  // Toggle LED state
        }
      }
    }

    // Close the connection
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
