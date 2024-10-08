#ifdef ESP32
  #include <WiFi.h>
  #include <WiFiClientSecure.h>
#else
  #include <ESP8266WiFi.h>
  #include <WiFiClientSecure.h>
#endif

String readString;
const char* ssid = "";           // Your WiFi SSID
const char* password = "";       // Your WiFi password
const int buttonPin = 2;         // Pushbutton pin
int buttonState;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
const char* host = "script.google.com";
const int httpsPort = 443;

#ifdef ESP32
  WiFiClientSecure client;
#else
  BearSSL::WiFiClientSecure client;
#endif

// SHA1 fingerprint of the certificate
const char* fingerprint = "46 B2 C3 44 9C 59 09 8B 01 B6 F8 BD 4C FB 00 74 91 2F EF F6";

// Replace with your GAS service ID
String GAS_ID = "AKfycbyw_TSyh_tPjvUloug9NVmPsny9eQUH3gf9zntWaHyMXKLbDHTV";

void setup() 
{
  pinMode(buttonPin, INPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // WiFi initialization for ESP32/ESP8266
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  sendData(200, 201, 202, 203, 204, 205, 206, 2007);  // Send test data
}

void loop() 
{
  int reading = digitalRead(buttonPin);

  // Debouncing logic
  if (reading != lastButtonState) {
    lastDebounceTime = millis();  // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      // Send data when button is pressed
      if (buttonState == HIGH) {
        sendData(100, 101, 102, 103, 104, 105, 106, 1007);
      }
    }
  }

  lastButtonState = reading;

  while (Serial.available()) {
    char c = Serial.read();  // Get one byte from serial buffer
    readString += c;  // Append to readString
    delay(2);  // Slow looping to allow buffer to fill with next character
  }

  if (readString.length() > 0) {
    Serial.println(readString);  // Show captured string
    // Convert readString into numbers (adjust logic as needed)
    sendData(analogRead(A0), readString.toInt(), readString.toInt(), readString.toInt(), readString.toInt(), readString.toInt(), readString.toInt(), readString.toInt());
    readString = "";  // Clear string for next input
  }
}

// Function to send data to Google Spreadsheet
void sendData(int tem, int hum, int tem1, int hum1, int tem2, int hum2, int tem3, int hum3)
{
  Serial.print("Connecting to ");
  Serial.println(host);

  // ESP32 uses setInsecure() to ignore certificate validation for development purposes
#ifdef ESP32
  client.setInsecure();  // Use this for development/testing
#else
  client.setFingerprint(fingerprint);  // For ESP8266, set the certificate fingerprint
#endif

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection failed");
    return;
  }

  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + String(tem) + "&humidity=" + String(hum) +
               "&temperature1=" + String(tem1) + "&humidity1=" + String(hum1) +
               "&temperature2=" + String(tem2) + "&humidity2=" + String(hum2) +
               "&temperature3=" + String(tem3) + "&humidity3=" + String(hum3);
  
  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("Request sent");

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("Headers received");
      break;
    }
  }

  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("ESP CI success!");
  } else {
    Serial.println("ESP CI failed");
  }

  Serial.println("Reply was:");
  Serial.println("==========");
  Serial.println(line);
  Serial.println("==========");
  Serial.println("Closing connection");

  client.stop();  // Close connection
}
