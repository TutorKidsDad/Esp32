
/*
https://www.callmebot.com/blog/whatsapp-messages-from-esp8266-esp32/
*/
#include <WiFi.h>         // Including WiFi.h library for WiFi tasks
#include <HTTPClient.h>   // Including HTTPClient.h library to use API
#include <RCSwitch.h>     // Including RCSwitch.h library to read RF data

RCSwitch mySwitch = RCSwitch();
#define RF_data_pin 23

const char* ssid = "Replace me with your SSID";             // WiFi SSID
const char* password =  "Replace me with your PASSWORD";    // WiFi password

String apiKey = "222222";              // Token number sent by bot on WhatsApp messenger
String phone_number = "+917561123123"; // WhatsApp registered phone number
long sensor_address = 16285605;        // 433 MHz wireless sensor unique address
String url;                            // String to store the final generated URL

unsigned long lastMessageTime = 0;     // Last time a message was sent
const unsigned long debounceDelay = 10000;  // 10 seconds delay between messages

int wifiRetryCount = 0;
const int wifiMaxRetries = 20;         // Max retry limit for WiFi connection

void setup() {
  Serial.begin(115200);
  mySwitch.enableReceive(RF_data_pin);

  // Connect to WiFi
  connectToWiFi();
}

void loop() {
  if (mySwitch.available()) {
    if (sensor_address == mySwitch.getReceivedValue()) {   // Compare the sensor address 
      if (millis() - lastMessageTime > debounceDelay) {    // Check debounce
        message_to_whatsapp(F("Alert!!! Someone Opened Your Door."));  // Send alert message
        lastMessageTime = millis();                        // Update the timestamp
      }
      delay(3000);                                         // Delay to avoid flooding
    }
    mySwitch.resetAvailable();  // Reset the data received
  }
}

// WiFi connection function with retry mechanism
void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print(F("Connecting to WiFi"));

  while (WiFi.status() != WL_CONNECTED && wifiRetryCount < wifiMaxRetries) {
    delay(500);
    Serial.print(F("."));
    wifiRetryCount++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("WiFi connected!"));
  } else {
    Serial.println(F("Failed to connect to WiFi. Rebooting..."));
    ESP.restart();  // Restart ESP32 if connection fails
  }
}

// Function to send message to WhatsApp
void message_to_whatsapp(String message) {
  // Create the complete URL
  url = "https://api.callmebot.com/whatsapp.php?phone=" + phone_number + "&apikey=" + apiKey + "&text=" + urlencode(message);

  postData();  // Call the API to send the message
}

// POST request to send data via API
void postData() {
  int httpCode;                  // Variable to store HTTP response code
  HTTPClient http;               // Create an HTTPClient object
  http.begin(url);               // Initialize HTTP client with the URL

  httpCode = http.POST(url);     // Send the POST request

  // Check for HTTP response codes
  if (httpCode > 0) {
    if (httpCode == 200) {
      Serial.println(F("Message sent successfully."));
    } else {
      Serial.print(F("HTTP Error: "));
      Serial.println(httpCode);
    }
  } else {
    Serial.print(F("HTTP request failed. Error: "));
    Serial.println(http.errorToString(httpCode).c_str());
  }

  http.end();  // Close the connection
}

// Function to URL encode the message
String urlencode(String str) {
  String encodedString = "";
  char c;
  char code0;
  char code1;

  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);

    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) {
        code1 = (c & 0xf) - 10 + 'A';
      }
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) {
        code0 = c - 10 + 'A';
      }
      encodedString += '%';
      encodedString += code0;
      encodedString += code1;
    }
    yield();  // Prevent watchdog timer reset
  }

  return encodedString;
}
