#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// Pin Definitions
const int motionPin = 27;        // GPIO pin for the motion sensor
const int led = 2;               // GPIO pin for the LED
const int potPin = 34;           // GPIO pin for the potentiometer

// State Variables
int state = LOW;                 // Current state of motion detection
int lastReading = LOW;           // Last reading from the motion sensor
unsigned long lastDebounceTime = 0; // Last time the motion state changed
const unsigned long debounceDelay = 200; // Debounce delay (200 ms)

// DFPlayer Setup
SoftwareSerial mySoftwareSerial(16, 17); // RX, TX for ESP32
DFRobotDFPlayerMini myDFPlayer;
unsigned long playbackStartMillis = 0;  // Time when playback started
bool isPlaying = false;                 // Flag to indicate if audio is playing

void setup() {
  pinMode(led, OUTPUT);          // Initialize LED as output
  pinMode(motionPin, INPUT);     // Initialize motion sensor pin as input
  pinMode(potPin, INPUT);        // Initialize potentiometer pin as input
  Serial.begin(115200);          // Initialize serial communication
  mySoftwareSerial.begin(9600);  // Communication with DFPlayer

  // Initialize DFPlayer
  Serial.println("Initializing DFPlayer Mini...");
  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println("DFPlayer not working! Please check connections.");
    return; // Gracefully exit if DFPlayer fails to initialize
  }
  
  Serial.println("DFPlayer Mini is online.");
  myDFPlayer.volume(20);         // Set volume level (0~30)
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);  // Set equalizer
  Serial.println("DFPlayer initialized with volume level 20.");

  randomSeed(analogRead(0));     // Seed random number generator
}

void loop() {
  int reading = digitalRead(motionPin); // Read motion sensor value

  // Check for stable state change
  if (reading != lastReading) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  // Only update state if stable for debounce period
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != state) {
      state = reading; // Update state

      if (state == HIGH) { // Motion detected
        Serial.println("Motion detected, attempting to play random file...");
        digitalWrite(led, HIGH); // Turn LED ON
        playRandomFile();        // Play a random audio file
        playbackStartMillis = millis(); // Record playback start time
        isPlaying = true;        // Set playback flag
      } else { // Motion stopped
        if (isPlaying) { // If playback is ongoing
          unsigned long playbackDuration = readPotentiometer(); // Get playback duration from pot
          if (millis() - playbackStartMillis >= playbackDuration) {
            myDFPlayer.stop(); // Stop playback after duration
            Serial.println("Audio playback stopped.");
            isPlaying = false; // Reset playback flag
          }
        }
        digitalWrite(led, LOW); // Turn LED OFF
      }
    }
  }

  lastReading = reading; // Update last reading
  delay(100); // Short delay to prevent rapid triggering
}

// Function to play a random audio file
void playRandomFile() {
  Serial.println("Attempting to read file count from SD card...");

  unsigned long startMillis = millis(); // Get the current time
  int fileCount = -1;

  // Try to get the file count with a timeout mechanism
  while ((millis() - startMillis) < 2000) { // Wait up to 2 seconds
    fileCount = myDFPlayer.readFileCounts(); // Try to get the total number of files
    if (fileCount >= 0) {
      if (fileCount > 0) {
        Serial.print("File count: ");
        Serial.println(fileCount);
        int randomFile = random(1, fileCount + 1); // Select random file to play
        Serial.print("Random file chosen: ");
        Serial.println(randomFile);
        myDFPlayer.play(randomFile); // Play the random file
        return; // Exit function after starting playback
      } else {
        Serial.println("No files found on SD card.");
        return; // Exit function since there are no files
      }
    }
  }

  // If file count could not be read or SD card is not found
  Serial.println("Error: Unable to read file count or SD card not found. Skipping playback.");
}

// Function to read the potentiometer value and map it to playback duration
unsigned long readPotentiometer() {
  int potValue = analogRead(potPin); // Read the potentiometer
  // Map the pot value (0-4095) to a desired playback duration range (1000ms to 30000ms)
  return map(potValue, 0, 4095, 1000, 30000); // Map to 1s to 30s
}
