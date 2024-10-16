/*
 * ESP32 + MQTT + DHT for IoT
 * V 1.1 - Ricardo Merces (Modified for ESP32)
 */

// Depends on the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - MQTT Client Library: https://github.com/knolleary/pubsubclient

// --- WiFi ---
#include <WiFi.h>  // For ESP32, use WiFi.h
const char* wifiSSID = "OnlineTPS";                  // Your SSID       
const char* wifiPassword = "TPS@12345";              // Your Password
WiFiClient espClient;

// --- MQTT ---
#include <PubSubClient.h>
const char* mqttServer = "broker.mqttdashboard.com";  // MQTT Broker (you can change it)
const char* mqttUsername = "swapnil";                 // MQTT Username
const char* mqttPassword = "";              // MQTT Password
const char* mqttClientId = "clientId-J6zsTinydq";     // MQTT Client ID (should be unique)
PubSubClient client(espClient);

// --- MQTT Topics ---
const char* temperatureTopic = "MQTT_TOPIC_TEMP";     // Topic for Temperature
const char* humidityTopic = "MQTT_TOPIC_HUMID";       // Topic for Humidity

// --- DHT ---
#include <DHT.h>
#define DHTTYPE DHT22                                // Update this to match your DHT type
const int dhtPin = 15;                               // ESP32 GPIO pin connected to the DHT sensor (use GPIO15 here)
const long sampleDelay = 1000;                       // Sampling delay in milliseconds
DHT dht(dhtPin, DHTTYPE);
unsigned long lastSampleTime = 0;

// --- SETUP ---
void setup() {
  Serial.begin(115200);  // Start serial communication
  setupWifi();
  client.setServer(mqttServer, 1883);  // Set MQTT broker
  dht.begin();  // Initialize DHT sensor
}

// --- LOOP ---
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();  // Keep the MQTT client connected
  measurementTempHumid();  // Measure temperature and humidity
}

// --- AUXILIARY FUNCTIONS ---

// --- SETUP WiFi ---
void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiSSID);

  WiFi.mode(WIFI_STA);  // Set WiFi mode to Station (client)
  WiFi.begin(wifiSSID, wifiPassword);  // Start WiFi connection

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  // Print the device's IP address
}

// --- MQTT-RECONNECT CLIENT --- 
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqttClientId, mqttUsername, mqttPassword)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);  // Wait 5 seconds before retrying
    }
  }
}

// --- MEASUREMENT TEMPERATURE & HUMIDITY ---
void measurementTempHumid() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastSampleTime >= sampleDelay) {
    lastSampleTime = currentMillis;
    float humidity = dht.readHumidity();
    float celsius = dht.readTemperature(false);

    // Check if any reads failed and exit early
    if (isnan(humidity) || isnan(celsius)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    client.publish(temperatureTopic, String(celsius).c_str(), true);   // Publish temperature to the MQTT topic
    Serial.print("Temperature: ");
    Serial.print(celsius);
    Serial.println(" *C");

    client.publish(humidityTopic, String(humidity).c_str(), true);     // Publish humidity to the MQTT topic
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }
}
