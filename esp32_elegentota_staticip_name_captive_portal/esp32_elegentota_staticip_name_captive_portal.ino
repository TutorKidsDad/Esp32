#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <EEPROM.h>
#include <Arduino.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include <ESPmDNS.h>

// WiFi Credentials structure for EEPROM storage
struct WiFiCredentials {
  char ssid[32];
  char password[64];
};

WiFiCredentials credentials;

// Variables
int statusCode;
String esid;
String epass = "";
String st;
String content;

// WiFi and server configurations
WebServer server(80);
const char* apSSID = "";
const char* apPassword = "";

// Timing variables for non-blocking delays
unsigned long previousMillis = 0;
const long interval = 1000; // Interval for WiFi status check
unsigned long scanStartTime = 0;
const long scanInterval = 30000; // WiFi scan every 30 seconds

// Function Declaration
bool testWifi(void);
void launchWeb(void);
void setupAP(void);
void createWebServer(void);
void saveCredentials(const String &ssid, const String &pass);
void loadCredentials(void);

void setup() {
  Serial.begin(115200);
  Serial.println("Disconnecting current WiFi connection");
  WiFi.disconnect();
  
  EEPROM.begin(512);
  pinMode(15, INPUT);
  
  Serial.println("Startup");
  
  // Load credentials from EEPROM
  loadCredentials();
  
  Serial.println("SSID from EEPROM: " + esid);
  Serial.println("PASS from EEPROM: " + epass);
  
  // Start WiFi connection
  WiFi.begin(esid.c_str(), epass.c_str());
}

void loop() {
  bool wifiConnected = (WiFi.status() == WL_CONNECTED);

  if (wifiConnected) {
    for (int i = 0; i < 10; i++) {
      Serial.println("Connected to " + esid + " Successfully");
      delay(100);
    }
  }

  if (testWifi() && (digitalRead(15) != 1)) {
    Serial.println("Connection status positive");
    return;
  } else {
    Serial.println("Connection Status Negative / D15 HIGH");
    Serial.println("Turning the HotSpot On");
    setupAP();  // Setup HotSpot
  }

  // Non-blocking WiFi connection loop with timeout
  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startAttemptTime > 10000) {  // 10 seconds timeout
      Serial.println("Failed to connect, restarting...");
      ESP.restart();
    }
    if (millis() - previousMillis >= interval) {
      previousMillis = millis();
      Serial.print(".");
      server.handleClient();  // Handle server requests
    }
  }
}

// Function to test WiFi connection
bool testWifi() {
  int c = 0;
  while (c < 20) {
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }
    delay(500);
    Serial.print("*");
    c++;
  }
  Serial.println("\nConnect timed out, opening AP");
  return false;
}

// Setup Access Point and launch the web server
void setupAP() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  
  // WiFi scan logic with a debounce mechanism
  if (millis() - scanStartTime > scanInterval) {
    scanStartTime = millis();
    int n = WiFi.scanNetworks();
    Serial.println("Scan done, " + String(n) + " networks found");

    st = "<ol>";
    for (int i = 0; i < n; ++i) {
      st += "<li>" + String(WiFi.SSID(i)) + " (" + String(WiFi.RSSI(i)) + ")</li>";
    }
    st += "</ol>";
  }

  WiFi.softAP(apSSID, apPassword);
  Serial.println("Initializing SoftAP for WiFi credentials modification");

  launchWeb();
}

// Launch web server
void launchWeb() {
  Serial.println("WiFi connected");
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());

  createWebServer();
  server.begin();
  Serial.println("Server started");
}

// Create a simple web server for updating WiFi credentials
void createWebServer() {
  server.on("/", []() {
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content.reserve(512);
    content = "<!DOCTYPE HTML><html>Welcome to WiFi Credentials Update page";
    content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
    content += ipStr;
    content += "<p>" + st + "</p>";
    content += "<form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32>";
    content += "<input name='pass' length=64><input type='submit'></form></html>";
    server.send(200, "text/html", content);
  });

  server.on("/scan", []() {
    content = "<!DOCTYPE HTML><html>go back</html>";
    server.send(200, "text/html", content);
  });

  server.on("/setting", []() {
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0) {
      saveCredentials(qsid, qpass);
      content = "{\"Success\":\"Saved to EEPROM... Reset to boot into new WiFi\"}";
      statusCode = 200;
      ESP.restart();
    } else {
      content = "{\"Error\":\"404 not found\"}";
      statusCode = 404;
      Serial.println("Sending 404");
    }
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(statusCode, "application/json", content);
  });
}

// Save WiFi credentials to EEPROM
void saveCredentials(const String &ssid, const String &pass) {
  ssid.toCharArray(credentials.ssid, sizeof(credentials.ssid));
  pass.toCharArray(credentials.password, sizeof(credentials.password));
  EEPROM.put(0, credentials);
  EEPROM.commit();
  Serial.println("Credentials saved to EEPROM");
}

// Load WiFi credentials from EEPROM
void loadCredentials() {
  EEPROM.get(0, credentials);
  esid = String(credentials.ssid);
  epass = String(credentials.password);
}
