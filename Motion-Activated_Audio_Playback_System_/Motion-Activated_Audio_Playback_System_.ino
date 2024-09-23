#include <Arduino.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "driver/i2s.h"  // ESP32 I2S driver

SoftwareSerial mySoftwareSerial(16, 17); // RX, TX for ESP32
DFRobotDFPlayerMini myDFPlayer;

const int motionPin = 27;        // GPIO pin for the motion sensor
const int led = 2;               // GPIO pin for LED
const unsigned long playbackDuration = 15000; // Playback duration (15 seconds)
int state = LOW;                 // By default, no motion detected
int val = 0;                     // Variable to store sensor status

unsigned long playbackStartMillis = 0;  // To keep track of playback start time
bool isPlaying = false;                 // Playback status flag

// I2S pins for the MAX98357A amplifier
const int i2sBCLK = 26;          // Bit clock line (BCLK)
const int i2sLRC = 25;           // Left-right clock (LRC)
const int i2sDOUT = 22;          // Data out (DOUT)

void setup() {
  pinMode(led, OUTPUT);          // Initialize LED as an output
  pinMode(motionPin, INPUT);     // Initialize motion sensor pin as input
  Serial.begin(115200);          // Initialize serial communication

  // Setup I2S for MAX98357A amplifier
  Serial.println("Initializing I2S for MAX98357A...");
  
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX), // Master and Transmit mode
    .sample_rate = 44100,        // Sample rate
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // 16-bit per sample
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT, // 2-channel (stereo)
    .communication_format = I2S_COMM_FORMAT_I2S_MSB, // I2S format
    .intr_alloc_flags = 0,       // No specific interrupt flags
    .dma_buf_count = 8,          // Number of DMA buffers
    .dma_buf_len = 64,           // Length of each buffer
    .use_apll = false            // No APLL usage
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = i2sBCLK,       // BCLK pin
    .ws_io_num = i2sLRC,         // LRC pin
    .data_out_num = i2sDOUT,     // Data out pin
    .data_in_num = I2S_PIN_NO_CHANGE  // Not used
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);  // Install I2S driver
  i2s_set_pin(I2S_NUM_0, &pin_config);                  // Set I2S pins
  Serial.println("I2S initialized for MAX98357A.");

  Serial.println("Initializing DFPlayer Mini...");
  mySoftwareSerial.begin(9600);  // Communication with DFPlayer
  if (!myDFPlayer.begin(mySoftwareSerial)) {  // Initialize the DFPlayer
    Serial.println("DFPlayer not working! Please check connections.");
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
