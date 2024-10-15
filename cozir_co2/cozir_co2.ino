/*
  Log environmental data to CSV file on SD card using a DataLogger Shield.

  created 23 Feb 2015
  by Koos Looijesteijn
 
  This code is in the public domain.
    
  Upon first use, the sensor must be calibrated. If you don't get proper
  readings, do this: 1) Upload the SetToPolling sketch - Because the library
  is made for polling. 2) Move the sensor to an area with fresh air 3) Upload
  the Calibrate sketch 4) Upload this sketch again.
  
  For logging in Fahrenheit, just replace all instances of 'Celsius'.
*/

/****************************
 Libraries
/****************************/
#include <SD.h>
#include <Wire.h>

#include <TimeLib.h>
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t. Get it at https://www.pjrc.com/teensy/td_libs_DS1307RTC.html

#include <stdlib.h> // For float to string conversion
#include <SoftwareSerial.h> // For serial port communication
#include "cozir.h" // Makes it easy to poll data from the sensor


/****************************
 Variables and settings
/****************************/

// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 10;

// Set serial connection
SoftwareSerial nss(2,3);// Pin 2 = Arduino receiver pin (Rx), connect to sensor transmitter pin (Tx). Pin 3 = Arduino Tx, connect it to sensor Rx.
COZIR czr(&nss); // Pass the SoftwareSerial object by reference

// Time stamp
tmElements_t currentTime;

// Arrays for measurements (lowest, average, highest).
float temperatures[3];
float humidity[3];
float CO2[3];// To prevent rouding errors adding up in the average, the CO2 int data is converted to float.

// Count number of polls per log to calculate average temperature during log period. As you may start polling in the middle of a logging period, the number of polls in a log can be lower than 5.
int pollsInLog = 0;

/****************************
 Setup
/****************************/
void setup(){
  Serial.begin(9600);// Start communication over serial port.
  pinMode(10, OUTPUT);

  if (!SD.begin(chipSelect)) {// see if the card is present and can be initialized:
    Serial.println(F("Something's wrong with SD card"));
  }
  delay(5000);// Sensor startup time.
}

/****************************
 Loop
/****************************/
void loop(){
  //We poll more often than we log, so we can register some extremes occurring during the logging intervals without having to log huge amounts of data.
  
  RTC.read(currentTime);// Get time from real time clock
  int currentMinute = currentTime.Minute;
  static int lastPoll;// last poll moment
  static int lastLog;// last log moment
  
  // Poll every minute
  if ((currentMinute - lastPoll < 59) && (currentMinute > lastPoll)){
    pollData();
    lastPoll = currentMinute;      
    if (currentMinute >= 59){
      lastPoll = 0;
    }
  }
  // Log every 15 minutes
  if (((currentMinute == 15) || (currentMinute == 30) || (currentMinute == 45) || (currentMinute == 0)) && (currentMinute != lastLog)){ 
  //if ((currentMinute > lastLog + 2) && (currentMinute != lastLog)){ 
    logData(currentTime);
    lastLog = currentMinute;
  }
  delay(100);// Delay prevents serial commands getting chopped up in separate characters.
}

/****************************
 Functions called from Loop
/****************************/

// Polling - requesting measurements from the sensor
void pollData(){
  pollsInLog++;
  // Measurements are pushed into arrays for each measurement type:

  // Save temperature
  float currentTemperature = czr.celsius();
  if ((currentTemperature < temperatures[0]) || (pollsInLog == 1)){// save temperature if it's the lowest value in this interval.
    temperatures[0] = currentTemperature; 
  }
  temperatures[1] = (temperatures[1] * (pollsInLog - 1)/pollsInLog) + (currentTemperature / pollsInLog);// calculate and save average
  if (currentTemperature > temperatures[2]){// save temperature if it's the highest value in this interval.
    temperatures[2] = currentTemperature; 
  }
  // Save humidity
  float currentHumidity = czr.humidity();
  if ((currentHumidity < humidity[0]) || (pollsInLog == 1)){// save humidity if it's the lowest value in this interval.
    humidity[0] = currentHumidity; 
  }
  humidity[1] = (humidity[1] * (pollsInLog - 1)/pollsInLog) + (currentHumidity / pollsInLog);// calculate and save average
  if (currentHumidity > humidity[2]){// save humidity if it's the highest value in this interval.
    humidity[2] = currentHumidity; 
  }
  // Save CO2
  float currentCO2 = (float)czr.CO2();
  if ((currentCO2 < CO2[0]) || (pollsInLog == 1)){// save CO2 if it's the lowest value in this interval.
    CO2[0] = currentCO2; 
  }
  CO2[1] = (CO2[1] * (pollsInLog - 1)/pollsInLog) + (currentCO2 / pollsInLog);// calculate and save average
  if (currentCO2 > CO2[2]){// save CO2 if it's the highest value in this interval.
    CO2[2] = currentCO2; 
  }
  return;
}

// Logging of measurements
void logData(tmElements_t currentTime){
  File dataFile;
  dataFile = SD.open("datalog.csv", FILE_WRITE);// Create file if it doesn't already exist.
  dataFile.close();
  dataFile = SD.open("datalog.csv");// Open the file
  if (dataFile) {
    String firstWord = getFirstWord(dataFile);
    dataFile.close();
    if (firstWord.length() == 0){// Empty file
      addTableHeader(dataFile);     
    }
    if (firstWord == "Time"){// First word matches the first word in the table header. Good enough check :)
      writeToSD(dataFile, currentTime);
    }
    if ((firstWord != "Time") && (firstWord.length() > 0)){
      Serial.println(F("Something's wrong with datalog.csv"));
    }
  }
  else {// if the file didn't open, print an error:
    Serial.println(F("error opening datalog.csv"));// This error may shows too if sram is full and firstWord isn't correctly evaluated.
  }
  return;
}

// Write a line with log data to a file on the SD card
void writeToSD(File dataFile, tmElements_t currentTime){
  dataFile = SD.open("datalog.csv", FILE_WRITE);

  dataFile.print(twoDigits(currentTime.Day));
  dataFile.print(F("-"));
  dataFile.print(twoDigits(currentTime.Month));
  dataFile.print(F("-"));
  dataFile.print(tmYearToCalendar(currentTime.Year));
  dataFile.print(F(" ")); 
  dataFile.print(currentTime.Hour);
  dataFile.print(F(":"));
  dataFile.print(twoDigits(currentTime.Minute));
  dataFile.print(F(":"));
  dataFile.print(twoDigits(currentTime.Second));
  dataFile.print(F(","));
  dataFile.print(floatToString(temperatures[0]) + ",");  
  dataFile.print(floatToString(temperatures[1]) + ",");  
  dataFile.print(floatToString(temperatures[2]) + ",");  
  dataFile.print(floatToString(humidity[0]) + ",");  
  dataFile.print(floatToString(humidity[1]) + ",");  
  dataFile.print(floatToString(humidity[2]) + ",");  
  dataFile.print(floatToString(CO2[0]) + ",");  
  dataFile.print(floatToString(CO2[1]) + ",");
  dataFile.print(floatToString(CO2[2]) + ",");  
  dataFile.print(String(pollsInLog) + "\r\n");

  dataFile.close();
  resetPolls();//Erase the values of the interval that is now saved to the file.  
  return;
}

// Converts floats into strings.
String floatToString(float f){
  char buff[10];
  String s = "";
  dtostrf(f, 4, 1, buff);// Convert float to string (variable, mininum width, precision, buffer)
  s += buff;
  return s;
}

// Clear arrays for new logging period
void resetPolls(){
  pollsInLog = 0;// Reset counter
  byte i = 0;
  while( i < 4 ){// Set all values in arrays to 0:
    temperatures[i] = 0;
    humidity[i] = 0;
    CO2[i] = 0;
    i++;
  }
  return;
}

// Return a string of the first four characters in a file
String getFirstWord(File dataFile){
  char chars[4];
  String firstWord;
  byte i = 0;
  while( i < 4 ){
    chars[i] = dataFile.read();
    int j = chars[i];
    if (j != -1){// When end of file is reached, -1 is returned.
      firstWord += String(chars[i]);
    }
    i++;
  }
  return firstWord;
}

// Write a line with table headers to a file
void addTableHeader(File dataFile){
  dataFile = SD.open("datalog.csv", FILE_WRITE);
  dataFile.println(F("Time,Temperature (low),Temperature,Temperature (high),Humidity (low),Humidity,Humidity (high),CO2 (low),CO2,CO2 (high),Measurements"));
  dataFile.close();
  return;
}

// Convert a single digit number to a string with that number preceded by 0: 1:1 pm because 1:01 pm.
String twoDigits(int number) {
  String digits;
  if (number >= 0 && number < 10) {
    digits = "0";
  }
  return digits += number;
}
