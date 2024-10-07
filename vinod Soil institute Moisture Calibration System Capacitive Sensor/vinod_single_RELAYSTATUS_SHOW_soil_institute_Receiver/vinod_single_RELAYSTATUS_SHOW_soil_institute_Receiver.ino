/* Developed by Suyash Dwivedi for TPS */
#include <WiFi.h>
#include <ThingSpeak.h>
#include <PubSubClient.h>

// WiFi Credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Define relay pin and other constants
const int RELAY_PIN = 12;
WiFiClient client;
PubSubClient mqttClient(client);

#include "ThingSpeak.h"
#include "secrets.h"

// Pin Definitions
#define Pump 26
#define Wifi_Status 27

// ThingSpeak Channel Details
unsigned long Read_Channel1_Number = SECRET_CH1_ID_READ;
const char* Channel_1_Read_APIKey = SECRET_READ1_APIKEY;
const char* Channel_1_Write_APIKey = SECRET_WRITE1_APIKEY;

unsigned long WRITE_Channel8_Number = SECRET_CH8_ID_WRITE;
const char* Channel_8_Write_APIKey = SECRET_WRITE8_APIKEY;

unsigned long SettingChannelNumber = SECRET_CH_ID_SETTING;
const char* mySettingReadAPIKey = SECRET_READ_APIKEY_SETTING;

unsigned int SettingFieldNumber1_High = 1;
unsigned int SettingFieldNumber1_Low = 2;

// Field Numbers for Zone A
unsigned int Read_Field_A[] = {1, 2, 3, 4, 5, 6, 7, 8};

bool relayStatus = false;

// Function to connect to WiFi
void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    digitalWrite(Pump, LOW);      // Ensure pump is off during WiFi connection attempts
    digitalWrite(Wifi_Status, LOW); // Indicate no WiFi connection
  }
  Serial.println("\nConnected to WiFi");
  digitalWrite(Wifi_Status, HIGH); // Indicate successful WiFi connection
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(Pump, OUTPUT);
  pinMode(Wifi_Status, OUTPUT);

  // Initialize all to LOW
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(Pump, LOW);
  digitalWrite(Wifi_Status, LOW);

  connectToWiFi();
}

void controlRelay(bool state) {
  if (state) {
    digitalWrite(RELAY_PIN, HIGH);
    relayStatus = true;
    Serial.println("Relay ON");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    relayStatus = false;
    Serial.println("Relay OFF");
  }
}

void controlPump(bool state) {
  if (state) {
    digitalWrite(Pump, HIGH);
    Serial.println("Pump ON");
    ThingSpeak.writeField(WRITE_Channel8_Number, 7, 1, Channel_8_Write_APIKey);
  } else {
    digitalWrite(Pump, LOW);
    Serial.println("Pump OFF");
    ThingSpeak.writeField(WRITE_Channel8_Number, 7, 0, Channel_8_Write_APIKey);
  }
}

// Function to read data from ThingSpeak
long readThingSpeakField(unsigned long channel, unsigned int field, const char* apiKey) {
  return ThingSpeak.readLongField(channel, field, apiKey);
}

void updateThingSpeak(int field, int value) {
  ThingSpeak.writeField(Read_Channel1_Number, field, value, Channel_1_Write_APIKey);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  long Setting1_High = readThingSpeakField(SettingChannelNumber, SettingFieldNumber1_High, mySettingReadAPIKey);
  long Setting1_Low = readThingSpeakField(SettingChannelNumber, SettingFieldNumber1_Low, mySettingReadAPIKey);

  long count3A1 = readThingSpeakField(Read_Channel1_Number, Read_Field_A[2], Channel_1_Read_APIKey); // Soil Moisture3 ZONE-A UP

  if (count3A1 < Setting1_Low) {
    controlRelay(true);
    updateThingSpeak(8, 1);  // Indicate relay on in ThingSpeak
  } else if (count3A1 > Setting1_High) {
    controlRelay(false);
    updateThingSpeak(8, 0);  // Indicate relay off in ThingSpeak
  }

  // Control pump based on relay status
  controlPump(relayStatus);

  delay(15000); // Avoid rapid ThingSpeak calls
}
