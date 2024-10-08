// Include libraries
#include <HardwareSerial.h>

// Check if we're using ESP32 or ESP8266
#ifdef ESP32
  HardwareSerial SIM900(1); // Use hardware serial port 1 for ESP32 (TX = GPIO 1, RX = GPIO 3)
#else
  #include <SoftwareSerial.h>
  SoftwareSerial SIM900(7, 8); // Use software serial for ESP8266
#endif

// Variable to store text message
String textMessage;

// Create a variable to store Lamp state
String lampState = "HIGH";

// Relay connected to pin 12
const int relay = 12;

void setup() {
  // Automatically turn on the shield
  pinMode(9, OUTPUT); // Pin 9 used to power the GSM module
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
  
  // Set relay as OUTPUT
  pinMode(relay, OUTPUT);

  // By default, the relay is off
  digitalWrite(relay, HIGH);
  
  // Initializing serial communication
  Serial.begin(19200);
#ifdef ESP32
  SIM900.begin(19200, SERIAL_8N1, 3, 1); // For ESP32, set RX and TX pins if necessary
#else
  SIM900.begin(19200);
#endif

  // Give time for the GSM shield to log on to the network
  delay(20000);
  Serial.println("SIM900 ready...");

  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r");
  delay(100);
  // Set module to send SMS data to serial out upon receipt 
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);
}

void loop() {
  // Check for incoming data from SIM900
  if (SIM900.available() > 0) {
    textMessage = SIM900.readString();
    Serial.print(textMessage);
    delay(10);
  }

  // Check the text message content
  if (textMessage.indexOf("ON") >= 0) {
    // Turn on relay and save current state
    digitalWrite(relay, LOW);
    lampState = "on";
    Serial.println("Relay set to ON");
    textMessage = "";
  }
  
  if (textMessage.indexOf("OFF") >= 0) {
    // Turn off relay and save current state
    digitalWrite(relay, HIGH);
    lampState = "off";
    Serial.println("Relay set to OFF");
    textMessage = "";
  }
  
  if (textMessage.indexOf("STATE") >= 0) {
    String message = "Lamp is " + lampState;
    sendSMS(message);
    Serial.println("Lamp state requested");
    textMessage = "";
  }
}

// Function that sends SMS
void sendSMS(String message) {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r");
  delay(100);

  // REPLACE THE X's WITH THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT+CMGS=\"XXXXXXXXXX\""); 
  delay(100);
  
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
  delay(5000);
}
