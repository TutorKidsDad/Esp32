#ifndef AWS_CONFIG_H
#define AWS_CONFIG_H

// Common AWS IoT settings
const char* AWS_IOT_ENDPOINT = "your-aws-endpoint.amazonaws.com";  // Replace with your AWS IoT endpoint
const int MQTT_PORT = 8883;

// Certificate and keys (replace with your own)
const char* AWS_CERT_CA = "-----BEGIN CERTIFICATE-----\nYOUR_ROOT_CA\n-----END CERTIFICATE-----";
const char* AWS_CERT_CRT = "-----BEGIN CERTIFICATE-----\nYOUR_DEVICE_CERTIFICATE\n-----END CERTIFICATE-----";
const char* AWS_CERT_PRIVATE = "-----BEGIN RSA PRIVATE KEY-----\nYOUR_PRIVATE_KEY\n-----END RSA PRIVATE KEY-----";

#endif // AWS_CONFIG_H

