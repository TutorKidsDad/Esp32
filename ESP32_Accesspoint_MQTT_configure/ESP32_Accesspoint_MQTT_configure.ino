#include <FS.h>  // For handling file system
#include <LittleFS.h>  // Use LittleFS instead of SPIFFS

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#include <ESPmDNS.h>  // Include mDNS for ESP32
#endif
#include <DNSServer.h>
#if defined(ESP8266)
#include <ESP8266WebServer.h>
#else
#include <WebServer.h>
#endif
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include <DHT.h>

#define DHTPIN 0  // DHT11 is connected to GPIO Pin 0
float humi;
float temp;

DHT dht(DHTPIN, DHT11);
WiFiClient client;

char mqtt_server[40];
char mqtt_port[6] = "1883";
char apiKey[16] = "YOUR_apiKey";  // Make it modifiable (char array)
char static_ip[16] = "10.0.1.56";
char static_gw[16] = "10.0.1.1";
char static_sn[16] = "255.255.255.0";
bool shouldSaveConfig = true;

void saveConfigCallback() {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("mounting FS...");
  
  // Use LittleFS instead of SPIFFS
  if (LittleFS.begin()) {
    Serial.println("mounted file system");
    if (LittleFS.exists("/config.json")) {
      Serial.println("reading config file");
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");
          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(apiKey, json["apiKey"]);

          if (json["ip"]) {
            Serial.println("setting custom IP from config");
            strcpy(static_ip, json["ip"]);
            strcpy(static_gw, json["gateway"]);
            strcpy(static_sn, json["subnet"]);
            Serial.println(static_ip);
          }
        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }

  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  IPAddress _ip, _gw, _sn;
  _ip.fromString(static_ip);
  _gw.fromString(static_gw);
  _sn.fromString(static_sn);
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);

  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5);
  WiFiManagerParameter custom_apiKey("blynk", "blynk token", apiKey, 16);

  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_apiKey);

  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  Serial.println("connected...yeey :)");

  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(apiKey, custom_apiKey.getValue());  // Use strcpy to copy the value

  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["apiKey"] = apiKey;
    json["ip"] = WiFi.localIP().toString();
    json["gateway"] = WiFi.gatewayIP().toString();
    json["subnet"] = WiFi.subnetMask().toString();

    File configFile = LittleFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }
    json.prettyPrintTo(Serial);
    json.printTo(configFile);
    configFile.close();
  }

  Serial.println("local IP:");
  Serial.println(WiFi.localIP());

  // Start mDNS service
  if (!MDNS.begin("esp32")) {  // Change "esp32" to your preferred hostname
    Serial.println("Error starting mDNS");
  } else {
    Serial.println("mDNS responder started");
  }
}

void loop() {
  humi = dht.readHumidity();
  temp = dht.readTemperature();
  
  if (client.connect(mqtt_server, 1883)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temp);
    postStr += "&field2=";
    postStr += String(humi);
    postStr += "\r\n\r\n";

    client.print(String("GET ") + postStr + " HTTP/1.1\r\n" +
                 "Host: " + mqtt_server + "\r\n" +
                 "Connection: close\r\n\r\n");
  }
  
  client.stop();
  delay(10000);
}
