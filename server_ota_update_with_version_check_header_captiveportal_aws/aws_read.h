#ifndef AWS_READ_H
#define AWS_READ_H

#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "aws_config.h"  // Include shared AWS configuration

// AWS IoT topic for reading data
const char* AWS_IOT_TOPIC_READ = "your/subscribe/topic";  // Replace with your MQTT topic

// WiFi client secured with certificates
WiFiClientSecure secureClient_read;
PubSubClient mqttClient_read(secureClient_read);

// Function to handle incoming messages from AWS IoT
void messageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);

  String message = "";
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Message: ");
  Serial.println(message);
}

// Function to initialize AWS connection for reading data
void initAWSRead() {
  secureClient_read.setCACert(AWS_CERT_CA);
  secureClient_read.setCertificate(AWS_CERT_CRT);
  secureClient_read.setPrivateKey(AWS_CERT_PRIVATE);

  mqttClient_read.setServer(AWS_IOT_ENDPOINT, MQTT_PORT);
  mqttClient_read.setCallback(messageReceived);

  Serial.print("Connecting to AWS IoT (for reading)...");

  while (!mqttClient_read.connected()) {
    if (mqttClient_read.connect("ESP32_Client_Read")) {
      Serial.println("Connected to AWS IoT (for reading).");

      // Subscribe to the topic to receive data
      if (mqttClient_read.subscribe(AWS_IOT_TOPIC_READ)) {
        Serial.println("Subscribed to AWS IoT topic.");
      } else {
        Serial.println("Failed to subscribe.");
      }
    } else {
      Serial.print("Failed to connect. State: ");
      Serial.println(mqttClient_read.state());
      delay(2000);
    }
  }
}

// Function to listen for incoming messages
void listenForAWSMessages() {
  mqttClient_read.loop();  // This function checks for any incoming messages
}

#endif // AWS_READ_H

