#include <WiFi.h>
#include <Espalexa.h>
#include <AceButton.h>
#include <WiFiManager.h> // WiFiManager Library
#include <ESPmDNS.h> // mDNS Library
using namespace ace_button;

// Device names
String DeviceNames[] = {
  "Study Lamp", "CFL", "Yellow light", "Night Lamp",
  "Studio Light", "Outdoor Light", "Kitchen Light", "Room Light"
};

// Pin Definitions
const uint8_t RelayPins[] = {23, 22, 21, 19, 18, 5, 25, 26};
const uint8_t SwitchPins[] = {13, 12, 14, 27, 33, 32, 15, 4};
const uint8_t wifiLedPin = 2; // D2

// Prototypes
void buttonHandler(AceButton* button, uint8_t eventType, uint8_t buttonState);
boolean connectWifi();

WiFiManager wm; // WiFiManager instance
Espalexa espalexa; // Declare the Espalexa object

boolean wifiConnected = false;
const char* mdnsName = "myesp32"; // mDNS hostname

void setup() {
  Serial.begin(115200);

  pinMode(wifiLedPin, OUTPUT);
  digitalWrite(wifiLedPin, LOW); // Turn off WiFi LED

  // Initialize WiFi
  wifiConnected = connectWifi();

  if (wifiConnected) {
    for (uint8_t i = 0; i < 8; i++) {
      pinMode(RelayPins[i], OUTPUT);
      digitalWrite(RelayPins[i], HIGH); // Turn off all relays
    }

    // Initialize mDNS
    if (!MDNS.begin(mdnsName)) {
      Serial.println("Error setting up mDNS responder!");
    } else {
      Serial.print("mDNS responder started. Visit http://");
      Serial.print(mdnsName);
      Serial.println(".local in your browser.");
    }

    // Add devices to ESPalexa
    espalexa.addDevice(DeviceNames[0], firstLightChanged);
    espalexa.addDevice(DeviceNames[1], secondLightChanged);
    // Add the other devices similarly
    espalexa.begin();
  } else {
    Serial.println("Cannot connect to WiFi. Operating in Manual Mode.");
    // Additional setup for manual mode can be added here
  }
}

void loop() {
  if (wifiConnected) {
    espalexa.loop();
  } else {
    // Additional loop code for manual mode can be added here
  }
}

boolean connectWifi() {
  // WiFi connection logic using WiFiManager
  wm.setMinimumSignalQuality(30); // Set minimum quality level
  wm.setAPCallback([](WiFiManager* wm) {
    Serial.println("Failed to connect, starting configuration portal...");
  });

  // Try to connect to the saved network, or start the configuration portal
  if (!wm.autoConnect("ESP32-Setup", "password123")) {
    Serial.println("Failed to connect and hit timeout. Rebooting...");
    delay(3000);
    ESP.restart(); // Restart if connection fails or timeout
    return false;
  }

  // Connected to Wi-Fi
  Serial.println("WiFi connected successfully!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(wifiLedPin, HIGH); // Turn on WiFi LED
  return true;
}

void setDeviceState(uint8_t deviceIndex, boolean state) {
  // Set the state of the specified device
  digitalWrite(RelayPins[deviceIndex], state ? LOW : HIGH);
  Serial.print("Device ");
  Serial.print(DeviceNames[deviceIndex]);
  Serial.println(state ? " ON" : " OFF");
}

void buttonHandler(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  int buttonIndex = button->getPin();
  uint8_t deviceIndex = -1;
  // Determine the device index based on the button pin
  for (uint8_t i = 0; i < 8; i++) {
    if (SwitchPins[i] == buttonIndex) {
      deviceIndex = i;
      break;
    }
  }
  if (deviceIndex != -1 && eventType == AceButton::kEventPressed) {
    // Toggle the state of the device
    boolean currentState = digitalRead(RelayPins[deviceIndex]) == LOW;
    setDeviceState(deviceIndex, !currentState);
  }
}

void firstLightChanged(uint8_t brightness) {
  if (brightness == 255) {
    setDeviceState(0, true);
  } else {
    setDeviceState(0, false);
  }
}

void secondLightChanged(uint8_t brightness) {
  if (brightness == 255) {
    setDeviceState(1, true);
  } else {
    setDeviceState(1, false);
  }
}

// Implement the other lightChanged functions similarly
