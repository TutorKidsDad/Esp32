#ifndef AWS_POST_H
#define AWS_POST_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "aws_config.h"  // Include shared AWS configuration

// AWS IoT topic for posting data
const char* AWS_IOT_TOPIC = "your/topic/name";  // Replace with your MQTT topic

// WiFi client secured with certificates
WiFiClientSecure secureClient;
PubSubClient mqttClient(secureClient);

// Function to initialize AWS connection for posting data
void initAWS() {
  secureClient.setCACert(AWS_CERT_CA);
  secureClient.setCertificate(AWS_CERT_CRT);
  secureClient.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient.setServer(AWS_IOT_ENDPOINT, MQTT_PORT);

  Serial.print("Connecting to AWS IoT...");

  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32_Client")) {
      Serial.println("Connected to AWS IoT");
    } else {
      Serial.print("Failed to connect. State: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

// Function to post data to AWS IoT
void postDataToAWS(float temperature, float humidity, int analogValue) {
  if (mqttClient.connected()) {
    String payload = "{";
    payload += "\"temperature\": " + String(temperature, 2) + ",";
    payload += "\"humidity\": " + String(humidity, 2) + ",";
    payload += "\"analogValue\": " + String(analogValue);
    payload += "}";

    Serial.print("Publishing data: ");
    Serial.println(payload);

    // Publish to AWS IoT topic
    if (mqttClient.publish(AWS_IOT_TOPIC, payload.c_str())) {
      Serial.println("Data published to AWS IoT.");
    } else {
      Serial.println("Failed to publish data.");
    }
  } else {
    Serial.println("Not connected to AWS IoT.");
  }
}

#endif // AWS_POST_H

