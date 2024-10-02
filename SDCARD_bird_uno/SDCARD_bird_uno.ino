#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>
#include <Wire.h>

// Pin Definitions
const int motionPin = 2;        // Pin for Microwave Motion Sensor (RCWL-0516)
const int led = 3;              // Pin for LED
const int potPin = A0;          // Pin for Potentiometer
const int speakerPin = 9;       // Pin for Speaker (PWM)

// SD card settings
const int chipSelect = 4;       // Pin for SD card module

// TMRpcm audio library object
TMRpcm audio;

// State Variables
int state = LOW;                // Current state of motion detection
int lastReading = LOW;          // Last reading from the motion sensor
unsigned long lastDebounceTime = 0;  // Last time motion state changed
const unsigned long debounceDelay = 200; // Debounce delay

// Variables to track playback
unsigned long playbackStartMillis = 0;  // Time when playback started
bool isPlaying = false;                 // Flag to indicate if audio is playing

void setup() {
  Serial.begin(115200);         // Initialize serial communication

  // Initialize pins
  pinMode(led, OUTPUT);         // Initialize LED as output
  pinMode(motionPin, INPUT);    // Initialize motion sensor as input
  pinMode(potPin, INPUT);       // Initialize potentiometer as input

  // Initialize TMRpcm
  audio.speakerPin = speakerPin; // Set speaker pin
  Serial.println("Initializing audio system...");

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD Card initialization failed!");
    return; // Stop execution if SD card fails
  }
  Serial.println("SD Card initialized successfully.");

  randomSeed(analogRead(A0));   // Seed the random generator using analog noise

  Serial.println("Setup complete, waiting for motion detection...");
}

void loop() {
  int reading = digitalRead(motionPin); // Read motion sensor value

  // Debouncing
  if (reading != lastReading) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  // Only update state if stable for debounce period
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != state) {
      state = reading; // Update state

      if (state == HIGH) { // Motion detected
        Serial.println("Motion detected!");
        digitalWrite(led, HIGH); // Turn LED on
        playRandomFile();        // Play random audio file
        playbackStartMillis = millis(); // Record playback start time
        isPlaying = true;        // Set playback flag
      } else { // Motion stopped
        if (isPlaying) {
          unsigned long playbackDuration = readPotentiometer(); // Get playback duration
          if (millis() - playbackStartMillis >= playbackDuration) {
            audio.disable(); // Stop playback
            Serial.println("Audio playback stopped.");
            isPlaying = false; // Reset playback flag
          }
        }
        digitalWrite(led, LOW); // Turn LED off
        Serial.println("Motion stopped, LED turned off.");
      }
    }
  }

  lastReading = reading; // Update last reading
  delay(100); // Small delay to avoid rapid triggering
}

void playRandomFile() {
  Serial.println("Attempting to open SD card root directory...");
  File root = SD.open("/");
  if (!root) {
    Serial.println("Failed to open SD card root directory.");
    return;
  }

  Serial.println("Files in the root directory:");
  File file = root.openNextFile();
  
  int fileCount = 0;

  // Count the files on the SD card and print each one
  while (file) {
    Serial.print("Found file: ");
    Serial.println(file.name()); // Print each file found
    fileCount++;
    file = root.openNextFile();
    delay(50); // Add a small delay to improve stability
  }
  root.close();

  Serial.print("Total files found: ");
  Serial.println(fileCount);

  if (fileCount == 0) {
    Serial.println("No files found on the SD card.");
    return;
  }

  int randomFileNumber = random(1, fileCount + 1);
  Serial.print("Playing random file number: ");
  Serial.println(randomFileNumber);

  // Reopen the root to play the random file
  root = SD.open("/");
  fileCount = 0;
  file = root.openNextFile();
  
  while (file) {
    fileCount++;
    if (fileCount == randomFileNumber) {
      Serial.print("Playing file: ");
      Serial.println(file.name());
      audio.play(file.name()); // Play the file with TMRpcm
      break;
    }
    file = root.openNextFile();
  }
  root.close();
  delay(100); // Add a delay after attempting to play the file
}


// Function to read the potentiometer and return playback duration
unsigned long readPotentiometer() {
  int potValue = analogRead(potPin); // Read potentiometer value
  unsigned long playbackDuration = map(potValue, 0, 1023, 1000, 30000); // Map to 1s to 30s
  Serial.print("Potentiometer value: ");
  Serial.print(potValue);
  Serial.print(" -> Playback duration: ");
  Serial.print(playbackDuration);
  Serial.println(" ms");
  delay(100); // Add a delay after reading the potentiometer
  return playbackDuration;
}
