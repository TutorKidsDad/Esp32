#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial mySoftwareSerial(16, 17); // RX, TX for ESP32
DFRobotDFPlayerMini myDFPlayer;

const int motionPin = 27;        // GPIO pin for the motion sensor
const int led = 2;               // GPIO pin for LED
const unsigned long playbackDuration = 15000; // Playback duration (15 seconds)
int state = LOW;                 // By default, no motion detected
int val = 0;                     // Variable to store sensor status

unsigned long playbackStartMillis = 0;  // To keep track of playback start time
bool isPlaying = false;                 // Playback status flag

void setup() {
  pinMode(led, OUTPUT);          // Initialize LED as an output
  pinMode(motionPin, INPUT);     // Initialize motion sensor pin as input
  Serial.begin(115200);          // Initialize serial communication

  Serial.println("Initializing DFPlayer Mini...");
  mySoftwareSerial.begin(9600);  // Communication with DFPlayer
  if (!myDFPlayer.begin(mySoftwareSerial)) {  // Initialize the DFPlayer
    Serial.println("DFPlayer not working! Please check connections.");
  //  while (true); // Halt if DFPlayer initialization fails
  }

  Serial.println("DFPlayer Mini is online.");
  
  myDFPlayer.volume(20);         // Set volume level (0~30)
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);  // Set equalizer
  Serial.println("DFPlayer initialized with volume level 20.");
}

void loop() {
  val = digitalRead(motionPin);  // Read motion sensor value

  if (val == HIGH) {             // If motion is detected
    Serial.println("Motion sensor triggered, checking state...");
    digitalWrite(led, HIGH);     // Turn LED ON
    Serial.println("LED turned ON due to motion.");

    if (state == LOW) {          // If motion state was LOW (no motion before)
      Serial.println("New motion detected, attempting to play random file...");
      state = HIGH;              // Update state to HIGH (motion detected)
      playRandomFile();          // Play random sound file
      playbackStartMillis = millis(); // Record playback start time
      isPlaying = true;         // Set playback flag
      
      Serial.print("Playing audio for ");
      Serial.print(playbackDuration / 1000);
      Serial.println(" seconds...");
    }
  } 
  else {                         // If no motion is detected
    if (state == HIGH && isPlaying) { // If the state was HIGH (motion detected before) and playback is ongoing
      if (millis() - playbackStartMillis >= playbackDuration) {
        myDFPlayer.stop();         // Stop the audio after 15 seconds
        Serial.println("Audio playback stopped after 15 seconds.");
        isPlaying = false;        // Reset playback flag
      }
    }
    
    if (state == HIGH) {         // If motion is detected but playback is not ongoing
      Serial.println("Motion stopped, resetting state and turning off LED.");
      digitalWrite(led, LOW);    // Turn LED OFF
      state = LOW;               // Update state to LOW (no motion)
      Serial.println("LED turned OFF.");
    }
  }

  delay(100);  // Short delay to prevent rapid triggering
}

void playRandomFile() {
  Serial.println("Attempting to read file count from SD card...");

  unsigned long startMillis = millis();   // Get the current time
  int fileCount = -1;

  // Try to get the file count with a timeout mechanism
  while ((millis() - startMillis) < 2000) {  // Wait up to 2 seconds for the file count
    fileCount = myDFPlayer.readFileCounts(); // Try to get the total number of files
    if (fileCount >= 0) {
      if (fileCount > 0) {
        Serial.print("File count: ");
        Serial.println(fileCount);
        int randomFile = random(1, fileCount + 1); // Select random file to play
        Serial.print("Random file chosen: ");
        Serial.println(randomFile);
        myDFPlayer.play(randomFile);  // Play the random file
        return; // Exit the function after successfully starting playback
      } else {
        Serial.println("No files found on SD card.");
        return; // Exit the function since there are no files
      }
    }
  }

  // If file count could not be read or SD card is not found
  Serial.println("Error: Unable to read file count or SD card not found. Skipping playback.");
}
