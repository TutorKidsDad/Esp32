#include <DNSServer.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <Preferences.h>

Preferences preferences;
DNSServer dnsServer;
AsyncWebServer server(80);

String ssid;
String password;
bool is_setup_done = false;
bool valid_ssid_received = false;
bool valid_password_received = false;
bool wifi_timeout = false;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="description" content="Wi-Fi setup page for IoT devices. Enter your Wi-Fi credentials to connect the device.">
  <title>Wi-Fi Setup</title>
  <style>
    :root {
      --primary-color: #007BFF;
      --hover-color: #0056b3;
      --background-color: #f0f0f0;
      --text-color: #333;
      --input-border-color: #ccc;
      --note-color: #888;
    }

    *, *::before, *::after {
      box-sizing: border-box;
    }

    body, html {
      margin: 0;
      padding: 0;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      background-color: var(--background-color);
      height: 100%;
      display: flex;
      align-items: center;
      justify-content: center;
    }

    main {
      background-color: #fff;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
      max-width: 400px;
      width: 100%;
    }

    h1 {
      color: var(--text-color);
      text-align: center;
      margin-bottom: 20px;
      font-size: 1.8rem;
    }

    form {
      display: grid;
      gap: 15px;
    }

    label {
      color: var(--text-color);
      font-size: 1rem;
    }

    input[type="text"], input[type="password"] {
      padding: 10px;
      border-radius: 5px;
      border: 1px solid var(--input-border-color);
      font-size: 1rem;
      width: 100%;
    }

    input[type="submit"] {
      padding: 12px;
      background-color: var(--primary-color);
      color: #fff;
      border: none;
      border-radius: 5px;
      font-size: 1rem;
      cursor: pointer;
      transition: background-color 0.3s ease;
    }

    input[type="submit"]:hover {
      background-color: var(--hover-color);
    }

    .note {
      text-align: center;
      font-size: 0.9rem;
      color: var(--note-color);
      margin-top: 10px;
    }

    footer {
      margin-top: 20px;
      text-align: center;
      font-size: 0.8rem;
      color: var(--note-color);
    }

    @media (max-width: 480px) {
      main {
        padding: 15px;
      }

      h1 {
        font-size: 1.5rem;
      }
    }
  </style>
</head>
<body>
  <main>
    <h1>Wi-Fi Setup</h1>
    <form action="/get" method="get" aria-label="Wi-Fi credentials form">
      <label for="ssid">SSID</label>
      <input type="text" id="ssid" name="ssid" placeholder="Enter your Wi-Fi SSID" required aria-required="true">

      <label for="password">Password</label>
      <input type="password" id="password" name="password" placeholder="Enter your Wi-Fi Password" required aria-required="true">

      <input type="submit" value="Connect">
    </form>
    <p class="note">Enter your Wi-Fi credentials to connect the device.</p>
  </main>
  <footer>
    &copy; 2024 IoT Device Captive Portal
  </footer>
</body>
</html>
)rawliteral";

class CaptiveRequestHandler : public AsyncWebHandler {
  public:
    CaptiveRequestHandler() {}
    virtual ~CaptiveRequestHandler() {}

    bool canHandle(AsyncWebServerRequest *request) {
      return true;
    }

    void handleRequest(AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
    }
};

void setupServer() {
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
    Serial.println("Client Connected");
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage;
    String inputParam;

    if (request->hasParam("ssid")) {
      inputMessage = request->getParam("ssid")->value();
      inputParam = "ssid";
      ssid = inputMessage;
      Serial.println(inputMessage);
      valid_ssid_received = true;
    }

    if (request->hasParam("password")) {
      inputMessage = request->getParam("password")->value();
      inputParam = "password";
      password = inputMessage;
      Serial.println(inputMessage);
      valid_password_received = true;
    }
    request->send(200, "text/html", "The values entered by you have been successfully sent to the device. It will now attempt WiFi connection");
  });
}

void WiFiSoftAPSetup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("esp-captive");
  Serial.print("AP IP address: "); Serial.println(WiFi.softAPIP());
}

void WiFiStationSetup(String rec_ssid, String rec_password) {
  wifi_timeout = false;
  WiFi.mode(WIFI_STA);
  char ssid_arr[20];
  char password_arr[20];
  rec_ssid.toCharArray(ssid_arr, rec_ssid.length() + 1);
  rec_password.toCharArray(password_arr, rec_password.length() + 1);
  Serial.print("Received SSID: "); Serial.println(ssid_arr); Serial.print("And password: "); Serial.println(password_arr);
  WiFi.begin(ssid_arr, password_arr);

  uint32_t t1 = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.print(".");
    if (millis() - t1 > 50000) //50 seconds elapsed connecting to WiFi
    {
      Serial.println();
      Serial.println("Timeout connecting to WiFi. The SSID and Password seem incorrect.");
      valid_ssid_received = false;
      valid_password_received = false;
      is_setup_done = false;
      preferences.putBool("is_setup_done", is_setup_done);

      StartCaptivePortal();
      wifi_timeout = true;
      break;
    }
  }
  if (!wifi_timeout)
  {
    is_setup_done = true;
    Serial.println("");  Serial.print("WiFi connected to: "); Serial.println(rec_ssid);
    Serial.print("IP address: ");  Serial.println(WiFi.localIP());
    preferences.putBool("is_setup_done", is_setup_done);
    preferences.putString("rec_ssid", rec_ssid);
    preferences.putString("rec_password", rec_password);
  }
}

void StartCaptivePortal() {
  Serial.println("Setting up AP Mode");
  WiFiSoftAPSetup();
  Serial.println("Setting up Async WebServer");
  setupServer();
  Serial.println("Starting DNS Server");
  dnsServer.start(53, "*", WiFi.softAPIP());
  server.addHandler(new CaptiveRequestHandler()).setFilter(ON_AP_FILTER); // only when requested from AP
  server.begin();
  dnsServer.processNextRequest();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  preferences.begin("my-pref", false);

  is_setup_done = preferences.getBool("is_setup_done", false);
  ssid = preferences.getString("rec_ssid", "Sample_SSID");
  password = preferences.getString("rec_password", "abcdefgh");

  if (!is_setup_done) {
    StartCaptivePortal();
  } else {
    Serial.println("Using saved SSID and Password to attempt WiFi Connection!");
    Serial.print("Saved SSID is "); Serial.println(ssid);
    Serial.print("Saved Password is "); Serial.println(password);
    WiFiStationSetup(ssid, password);
  }

  while (!is_setup_done) {
    dnsServer.processNextRequest();
    delay(10);
    if (valid_ssid_received && valid_password_received) {
      Serial.println("Attempting WiFi Connection!");
      WiFiStationSetup(ssid, password);
    }
  }

  Serial.println("All Done!");
}

void loop() {
  // Your Loop code
}
