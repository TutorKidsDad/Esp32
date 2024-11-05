#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>  // Include mDNS library

// Access Point credentials
const char* ssid = "ESP32_Timer_Control";
const char* password = "12345678";
const char* hostname = "timer";  // mDNS hostname

// Pins for the buttons
int buttonPin = 19;
int upButtonPin = 17;
int downButtonPin = 18;

// Countdown timer variables
int countdownTime = 60;  // Initial time for countdown in seconds
bool countingDown = false;  // Timer status
unsigned long countdownStartTime = 0;
unsigned long countdownTimeLeft = countdownTime * 1000;  // in milliseconds

// Web server on port 80
WebServer server(80);

void setup() {
  // Initialize serial monitor
  Serial.begin(115200);

  // Set up button pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);

  // Set up access point
  WiFi.softAP(ssid, password);
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  // Start mDNS service
  if (MDNS.begin(hostname)) {
    Serial.println("mDNS responder started");
    Serial.print("Access the web page at http://");
    Serial.print(hostname);
    Serial.println(".local");
  } else {
    Serial.println("Error starting mDNS");
  }

  // Define routes
  server.on("/", handleRoot);               // Main page
  server.on("/start", startCountdown);      // Start/Stop timer
  server.on("/up", incrementCountdown);     // Increase time
  server.on("/down", decrementCountdown);   // Decrease time

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();  // Handle client requests

  // Update countdown timer
  if (countingDown) {
    countdownTimeLeft = countdownTime * 1000 - (millis() - countdownStartTime);
    if (countdownTimeLeft <= 0) {
      countingDown = false;
      countdownTimeLeft = 0;
      Serial.println("Time's up!");
    }
  }
}


void handleRoot() {
  // Serve the HTML page with embedded CSS
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP32 Timer Control</title>
      <style>
        body {
          font-family: Arial, sans-serif;
          text-align: center;
          background-color: #f4f4f9;
          color: #333;
        }
        h2 { color: #4CAF50; margin-top: 20px; }
        .button { 
          display: inline-block;
          padding: 15px 25px;
          font-size: 16px;
          margin: 5px;
          color: white;
          background-color: #4CAF50;
          border: none;
          border-radius: 8px;
          cursor: pointer;
        }
        .button:hover { background-color: #45a049; }
      </style>
    </head>
    <body>
      <h2>ESP32 Timer Control</h2>
      <p>Time Left: <span id="timeLeft">0</span> seconds</p>
      <button class="button" onclick="location.href='/start'">Start/Stop</button><br>
      <button class="button" onclick="location.href='/up'">Up (+1s)</button>
      <button class="button" onclick="location.href='/down'">Down (-1s)</button>

      <script>
        // Auto-refresh time left every second
        setInterval(async () => {
          const response = await fetch('/timeleft');
          const timeLeft = await response.text();
          document.getElementById('timeLeft').innerText = timeLeft;
        }, 1000);
      </script>
    </body>
    </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void startCountdown() {
  if (countingDown) {
    // Stop the countdown timer
    countingDown = false;
    countdownTimeLeft = countdownTime * 1000;
  } else {
    // Start the countdown timer
    countingDown = true;
    countdownStartTime = millis();
  }
  server.sendHeader("Location", "/");  // Redirect back to main page
  server.send(303);
}

void incrementCountdown() {
  countdownTime++;
  countdownTimeLeft = countdownTime * 1000;
  server.sendHeader("Location", "/");  // Redirect back to main page
  server.send(303);
}

void decrementCountdown() {
  if (countdownTime > 1) {
    countdownTime--;
    countdownTimeLeft = countdownTime * 1000;
  }
  server.sendHeader("Location", "/");  // Redirect back to main page
  server.send(303);
}

void getCountdownTimeLeft() {
  // Send the remaining time in seconds
  server.send(200, "text/plain", String(countdownTimeLeft / 1000));
}
