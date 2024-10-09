#include <WiFi.h>
#include <ArduinoOTA.h>

#define LED1 26
#define LED2 27
#define LM35 33
#define CONNECT_TIME 10000
#define TIMEOUTTIME 5000

// Replace with your network credentials
const char* ssid     = "testing";
const char* password = "password";
WiFiServer server(80);

String header;
String LED1State = "off";
String LED2State = "off";

unsigned long currentTime = 0;
unsigned long previousTime = 0;
unsigned long lastLedToggle = 0;
const unsigned long debounceDelay = 200;  // 200ms debounce time

// Debouncing and LED control
void handleLEDControl(int ledPin, String& ledState, const String& newState) {
  if (millis() - lastLedToggle > debounceDelay) {
    lastLedToggle = millis();
    if (newState == "on") {
      digitalWrite(ledPin, HIGH);
      ledState = "on";
    } else {
      digitalWrite(ledPin, LOW);
      ledState = "off";
    }
  }
}

// Function to read temperature from LM35 with averaging
double readTemperature() {
  int readings = 10;
  double totalTemp = 0;

  for (int i = 0; i < readings; i++) {
    totalTemp += analogRead(LM35) / 9.31;
    delay(10); // Small delay between readings
  }

  return totalTemp / readings; // Return averaged temperature
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LM35, INPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  if (!WiFi.softAP(ssid, password)) {
    Serial.println("Failed to set up Wi-Fi Access Point!");
    while (1);  // Stop further execution
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  server.begin();

  // Initialize OTA updates
  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();  // Handle OTA updates

  WiFiClient client = server.available();  // Listen for incoming clients
  if (client) {
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");
    String currentLine = "";

    while (client.connected() && currentTime - previousTime <= TIMEOUTTIME) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Handle LED1 control
            if (header.indexOf("GET /LED1/on") >= 0) {
              Serial.println("LED1 on");
              handleLEDControl(LED1, LED1State, "on");
            } else if (header.indexOf("GET /LED1/off") >= 0) {
              Serial.println("LED1 off");
              handleLEDControl(LED1, LED1State, "off");
            }

            // Handle LED2 control
            if (header.indexOf("GET /LED2/on") >= 0) {
              Serial.println("LED2 on");
              handleLEDControl(LED2, LED2State, "on");
            } else if (header.indexOf("GET /LED2/off") >= 0) {
              Serial.println("LED2 off");
              handleLEDControl(LED2, LED2State, "off");
            }

            // Prepare the web page content
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<meta charset=\"UTF-8\">");
            client.println("<meta http-equiv=\"refresh\" content=\"5\" >");
            client.println("<link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css\">");
            client.println("<style>html {font-family: Helvetica; text-align: center;} .mybtn {padding: 16px 40px; font-size: 30px;}</style></head>");
            client.println("<body class=\"bg-light\">");
            client.println("<h1>ESP32 Web Server</h1>");

            // Display LED1 state
            client.println("<p>GPIO 26 - State: " + LED1State + "</p>");
            if (LED1State == "off") {
              client.println("<p><a href=\"/LED1/on\" class=\"btn btn-success mybtn\">ON</a></p>");
            } else {
              client.println("<p><a href=\"/LED1/off\" class=\"btn btn-danger mybtn\">OFF</a></p>");
            }

            // Display LED2 state
            client.println("<p>GPIO 27 - State: " + LED2State + "</p>");
            if (LED2State == "off") {
              client.println("<p><a href=\"/LED2/on\" class=\"btn btn-success mybtn\">ON</a></p>");
            } else {
              client.println("<p><a href=\"/LED2/off\" class=\"btn btn-danger mybtn\">OFF</a></p>");
            }

            // Display Temperature
            double temp = readTemperature();
            client.println("<p>LM35 - Temperature: " + String(temp) + "°C</p>");
            if (temp < 21) {
              client.println("<p><span style=\"background-color: #198754; color: #f8f9fa;\">Cool: " + String(temp) + "°C</span></p>");
            } else if (temp > 24) {
              client.println("<p><span style=\"background-color: #dc3545; color: #f8f9fa;\">Hot: " + String(temp) + "°C</span></p>");
            } else {
              client.println("<p><span style=\"background-color: #ffc107; color: #212529;\">Warm: " + String(temp) + "°C</span></p>");
            }

            client.println("</body></html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }

    header = "";
    client.stop();
    Serial.println("Client disconnected.");
  }

  // Non-blocking delay using millis()
  if (millis() - previousTime > debounceDelay) {
    previousTime = millis();
  }
}
