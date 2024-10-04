/*********
project reference https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/

flow of process
setup- set servo to initial position ie one open other close
      - linear actuators closed initially
         - stepper motor 2 at initial position
         
Loop - stepper motor 1 move clockwise
      - check if any plant is left (laser sensor)- if yes move back    
                                  - if no then this process is done
       - both linear actuators expand
       - plant side servo close (open in setup)     
       - dropside side servo open (close in setup)  
       - linear actuator close   
       - stepper motor 2 rotate clockwise towards drophole
       - plant side servo open  (close in above step) 
       - drop side servo close   (open in above step) 
       - stepper motor 2 rotate counter-clockwise towards plant
*********/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <Hash.h>
  #include <FS.h>
#endif
#include <ESPAsyncWebServer.h>

#include <LiquidCrystal_I2C.h>

// set the LCD number of columns and rows
int lcdColumns = 16;
int lcdRows = 2;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);  

//////////////////////
////////////////////////
AsyncWebServer server(80);

  // laser Sensor code for unpicked plant 
#define DETECT 34 // pin D34 for laser sensor
#define ACTION 14 // pin D4 for action to do someting like buzzer


/////////////////////////leafswitch///////////////
#define LEAFSWITCH 25 // pin D34 for laser sensor
 int switchpress = digitalRead(LEAFSWITCH);// read Laser sensor
 //int switchpress = 0;  // variable for reading the pushbutton status

////////////////doing process n times - number of columns //////////
int myCounter = 0;
int mySwitchPin = 36;  //reset pin to repeat the process when all plants are planted

/////////////////////////////////
#include <ESP32Servo.h>
Servo myservo1;  // create servo object to control a servo
Servo myservo2;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position
// GPIO the servo is attached to
//static const int servo1Pin = 13;
//static const int servo2Pin = 12;
int servo1Pin = 13;
int servo2Pin = 12;
//////////////////////////////////
#include <Stepper.h>

//stepper Motor Driver Pins
const int DIR1 = 19;
const int STEP1 = 18;
//const int  steps_per_rev1 = 200;
const int DIR2 = 5;
const int STEP2 = 16;
//const int  steps_per_rev2 = 200;
int i=0;
int enable1Pin = 4; 
int enable2Pin =17 ; 

// initialize the stepper library
/////////////////////////////
// DC Motor A
int motor1Pin1 = 27; 
int motor1Pin2 = 26; 

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
//int dutyCycle = 200;

//////////////////////////////

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "testing";
const char* password = "password";

//const char* PARAM_STRING = "inputString";
const char* PARAM_INT1 = "inputInt1"; ////////////STEPPER MOTOR 1 PUSHING MOTOR
const char* PARAM_INT2 = "inputInt2"; ////////////STEPPER MOTOR 2 TO AND FRO MOTOR
const char* PARAM_INT3 = "inputInt3"; ////////////LINEAR ACTUATOR MOTOR
const char* PARAM_INT4 = "inputInt4"; ////////////SERVO MOTOR
const char* PARAM_INT5 = "inputInt5"; ////////////COUNTER VALUE FOR NUMBER OF COLUMNS
//const char* PARAM_FLOAT = "inputFloat"; 

// HTML web page to handle 3 input fields (inputString, inputInt, inputFloat)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP SPIFFS");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
    
  </script></head><body>

  <form action="/get" target="hidden-form">
    inputInt1 (current value %inputInt1%): <input type="number " name="inputInt1">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>

    <form action="/get" target="hidden-form">
    inputInt2 (current value %inputInt2%): <input type="number " name="inputInt2">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>

    <form action="/get" target="hidden-form">
    inputInt3 (current value %inputInt3%): <input type="number " name="inputInt3">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>

      <form action="/get" target="hidden-form">
    inputInt4 (current value %inputInt4%): <input type="number " name="inputInt4">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>

     <form action="/get" target="hidden-form">
    inputInt5 (current value %inputInt5%): <input type="number " name="inputInt5">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()){
    Serial.println("- empty file or failed to open file");
    return String();
  }
  Serial.println("- read from file:");
  String fileContent;
  while(file.available()){
    fileContent+=String((char)file.read());
  }
  file.close();
  Serial.println(fileContent);
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, "w");
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
  file.close();
}

// Replaces placeholder with stored values
String processor(const String& var){
  //Serial.println(var);
  if(var == "inputInt1"){
    return readFile(SPIFFS, "/inputInt1.txt");
  }
  else if(var == "inputInt2"){
    return readFile(SPIFFS, "/inputInt2.txt");
  }
  else if(var == "inputInt3"){
    return readFile(SPIFFS, "/inputInt3.txt");
  }
   else if(var == "inputInt4"){
    return readFile(SPIFFS, "/inputInt4.txt");
  }
     else if(var == "inputInt5"){
    return readFile(SPIFFS, "/inputInt5.txt");
  }
  return String();
}

///////////////////////////////////////////
void stepmotor2init()
{
    for (int i = 0; (i < 200); i++)
{
   digitalWrite(enable2Pin, HIGH);
   
  Serial.println("moving towards leafswitch");
 Serial.println(i);
  int switchpress = digitalRead(LEAFSWITCH);// read leafswitch position

   if ( switchpress == HIGH) 
        {
           Serial.println("Hit the leafswitch -- DONE");
    digitalWrite(ACTION,HIGH);// set the buzzer ON
    Serial.println("LEAFSWITCH ACTIVATED");
    digitalWrite(enable2Pin, LOW); 
  digitalWrite(DIR1, LOW);
    Serial.println("Stepper motor 2 Halted ");
    delay(500); 
          break; 
          } 
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(2000);
     Serial.println("stepper motor 2 started moving towards leafswitch");
    //   delay(500);       
}
}
///////////////////////////////////////////

void setup() {
  Serial.begin(115200);
    pinMode(LEAFSWITCH, INPUT);
Serial.println("SETUP OF PROGRAM STARTED");
  switchpress = digitalRead(LEAFSWITCH);// read leafswitch
  //  buttonState = digitalRead(buttonPin);
  delay(100);

////////////////////////// initialize LCD///////////
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
    Serial.println("LCD initiated");
 //////////////////////////repeat n times for columns///
 pinMode(mySwitchPin, INPUT_PULLUP);
////////////////////////////////////  // Laser sensor  ////////////
  Serial.println("Undetected plant pins initiated");
  pinMode(DETECT, INPUT);//define detect input pin
  pinMode(ACTION, OUTPUT);//define ACTION output pin

 /////////////////////////LEAFSWITCH/////////////
//  pinMode(LEAFSWITCH, INPUT);//define LEAFSWITCH input pin
///////////////////////////////////////
  // Initialize SPIFFS
  #ifdef ESP32
    if(!SPIFFS.begin(true)){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #else
    if(!SPIFFS.begin()){
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
    }
  #endif

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
   // return;
  }
  Serial.println();
  Serial.println("WiFi function starting...");
  Serial.println("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
delay(5000);
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
       // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    if (request->hasParam(PARAM_INT1)) {
      inputMessage = request->getParam(PARAM_INT1)->value();
      writeFile(SPIFFS, "/inputInt1.txt", inputMessage.c_str());
    }
    // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_INT2)) {
      inputMessage = request->getParam(PARAM_INT2)->value();
      writeFile(SPIFFS, "/inputInt2.txt", inputMessage.c_str());
    }
       // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_INT3)) {
      inputMessage = request->getParam(PARAM_INT3)->value();
      writeFile(SPIFFS, "/inputInt3.txt", inputMessage.c_str());
    }
       // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_INT4)) {
      inputMessage = request->getParam(PARAM_INT4)->value();
      writeFile(SPIFFS, "/inputInt4.txt", inputMessage.c_str());
    }
         // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
    else if (request->hasParam(PARAM_INT5)) {
      inputMessage = request->getParam(PARAM_INT5)->value();
      writeFile(SPIFFS, "/inputInt5.txt", inputMessage.c_str());
    }
    else {
      inputMessage = "No message sent";
    }
    Serial.println(inputMessage);
    request->send(200, "text/text", inputMessage);
  });
  server.onNotFound(notFound);
  server.begin();
///////////////////////////////////DC Motor/////
 // sets the pins as outputs:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);

  // testing
  Serial.println("Initializing DC Motor setup....ok");
  ////////////////////////////BRING BOTH DC MOTOR TO CLOSE POSITION INTITALLY ////////////////
  // Move DC motor 1 AND 2 CLOSING at maximum speed  ////////////////
  Serial.println("BRING BOTH DC MOTOR TO CLOSE POSITION INTITALLY ");
  
 //  digitalWrite(enable1Pin, HIGH);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  delay(15000);

  // Stop the DC motor
  Serial.println("Motor stopped");
//  digitalWrite(enable1Pin, LOW);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);
  
////////////////////////////////////servo/////
  // Allow allocation of all timers
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
 myservo1.setPeriodHertz(50);    // standard 50 hz servo
    myservo2.setPeriodHertz(50);    // standard 50 hz servo
  myservo1.attach(servo1Pin, 500, 2400); // attaches the servo on pin 18 to the servo object
    myservo2.attach(servo2Pin, 500, 2400); // attaches the servo on pin 18 to the servo object

  // using default min/max of 1000us and 2000us
  // different servos may require different min/max settings
  // for an accurate 0 to 180 sweep

// myservo1.attach(13);  // attaches the servo on pin 13 to the servo object
//  myservo2.attach(12);  // attaches the servo on pin 13 to the servo object
  Serial.println("Initializing servo Motors setup....ok");
  
  ///////////////////////////////////STEPPER MOTOR////////////steps_per_rev/////
 
  pinMode(STEP1, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(STEP2, OUTPUT);
  pinMode(DIR2, OUTPUT);
   pinMode(enable1Pin, OUTPUT);
    pinMode(enable2Pin, OUTPUT);
    // testing
  Serial.println("Initializing STEPPER Motor setup....ok");
  
  ///////////////////////STEPPER MOTOR 2 BRINGING IT TO ZERO POSITION USE LIMIT SWITCH ///////////////
stepmotor2init();
  
  }
   ////////////////////////// STOP BY LEAFSWITCH STOP//////////

///////////////////////////////////SERVO MOTORS ZERO, LINEAR ACTUATOR ZERO AND STEPPER MOTOR ZERO POSITION DONE ///////////////

void loop() {
  Serial.println();
Serial.println("LOOP OF PROGRAM STARTED");
    // set cursor to first column, first row
  lcd.setCursor(0, 0);
  // print message
  lcd.print("Hello, User!");
  delay(1000);
  // clears the display to print new message
  lcd.clear();
  // set cursor to first column, second row
  lcd.setCursor(0,1);
  lcd.print("Initializing..");
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(0, 0);
    lcd.print("IP Address: ");
    lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  /////////////////////////
  // To access your stored values on inputString, inputInt, inputFloat
  int yourInputInt1 = readFile(SPIFFS, "/inputInt1.txt").toInt();
  Serial.print("*** Your inputInt1: ");
  Serial.println(yourInputInt1);
  
  int yourInputInt2 = readFile(SPIFFS, "/inputInt2.txt").toInt();
  Serial.print("*** Your inputInt2: ");
  Serial.println(yourInputInt2);
  
  int yourInputInt3 = readFile(SPIFFS, "/inputInt3.txt").toInt();
  Serial.print("*** Your inputInt3: ");
  Serial.println(yourInputInt3);

    int yourInputInt4 = readFile(SPIFFS, "/inputInt4.txt").toInt();
  Serial.print("*** Your inputInt4: ");
  Serial.println(yourInputInt4);

      int yourInputInt5 = readFile(SPIFFS, "/inputInt5.txt").toInt();
  Serial.print("*** Your inputInt5: ");
  Serial.println(yourInputInt5);
  
  delay(5000);
  /////////////////////////////////////////
  if(myCounter<yourInputInt5)
{
  ////////////////////////////////////////////////////////////////////////1st process start///////////////////////////////////////////////

////////////////////////STEPPER MOTOR 1 ///////////////steps_per_rev/////
   digitalWrite(enable1Pin, HIGH); 
  digitalWrite(DIR1, HIGH);
  Serial.println("Stepper motor 1 Spinning Clockwise...");
  Serial.println("Stepper motor number of steps...");
  Serial.println(yourInputInt1);
  for(int i = 0; i<yourInputInt1; i++)
  {
    digitalWrite(STEP1, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP1, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); 
  digitalWrite(enable1Pin, LOW); 
  
/////////////////////take back the seed tray ///////if plant detected///////////
  int detected = digitalRead(DETECT);// read Laser sensor
 
  if( detected == HIGH)
  {
    digitalWrite(ACTION,HIGH);// set the buzzer ON
    Serial.println("leftover plant Detected!");
    ////////////////////  
    digitalWrite(enable1Pin, HIGH); 
  digitalWrite(DIR1, LOW);
    Serial.println("Stepper motor 1 Spinning Anti-Clockwise...");
  Serial.println("Stepper motor number of steps...");
  Serial.println(yourInputInt1);
  for(int i = 0; i<yourInputInt1; i++)
  {
    digitalWrite(STEP1, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP1, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
  digitalWrite(enable1Pin, LOW); 
////////////////////////// STOP plant detected///////////

  }else{
    digitalWrite(ACTION,LOW); // Set the buzzer OFF
    Serial.println("No laser");
     // Laser Sensor code 

  }
  delay(200);

///////////////////////STEPPER MOTOR2 PART 1 END ///////////////

////////////////////////////////////////////////////////////////////////1st process done///////////////////////////////////////////////

//////////////////////////////////////////2nd process 1. rotate - 2 extend - 3 servo close///////initially servo should be open /////////////////////////////////////


 /////BOTH DONE HERE SIMULTANIOUSLY ///////both DC motor EXTANDING on both sides side ///////
                                                        
 // Move the DC motor 1 AND 2 EXPAND at maximum speed ////////////////
  Serial.println("Moving EXPAND");  
   //  digitalWrite(enable1Pin, HIGH);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH); 
  delay(yourInputInt3);

  // Stop the DC motor  //////////////////////both motors stopped //////////////////
  Serial.println("Motor stopped");
  //   digitalWrite(enable1Pin, LOW);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

                                                         ////////////opened servo closing to pick plant /////// 
                                                 
  for (pos = yourInputInt4; pos >= 0; pos -= 1) { // goes from desired degrees to 0 degrees
     Serial.println("servo 1 closing to pick plant in number of steps");
  Serial.println(yourInputInt4);
    myservo1.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }


 ////////////meanwhile opposite side servo opening to release plant /////// 

 
   for (pos = 0; pos <= yourInputInt4; pos += 1) { // goes from 0 degrees to desired degrees
    // in steps of 1 degree
     Serial.println("meanwhile otherside servo 2 opening to release plant in number of steps");
  Serial.println(yourInputInt4);
    myservo2.write(pos); 
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }


////////////////////////////BOTH DC motor CLOSING ////////////////


  // Move DC motor 1 AND 2 backwards at maximum speed  ////////////////
  Serial.println("Moving Backwards / RETRACING ");
  //   digitalWrite(enable1Pin, HIGH);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW); 
  delay(yourInputInt3);

  // Stop the DC motor
  Serial.println("Motor stopped");
   //  digitalWrite(enable1Pin, LOW);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  delay(1000);

////////////////////////////////////////////////////////////////////
  ///////////////////////STEPPER MOTOR 2 BEGIN TO MOVE ///////////////
digitalWrite(enable2Pin, HIGH); 
  digitalWrite(DIR2, HIGH);
    Serial.println("Stepper motor 2 Spinning Clockwise...");
  Serial.println("Stepper motor number of steps...");
  Serial.println(yourInputInt2);
  for(int i = 0; i<yourInputInt2; i++)
  {
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(2000);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); 
digitalWrite(enable2Pin, LOW); 
    ///////////////////////////////////////////////----------------------------------------                                                    
     
   for (pos = 0; pos <= yourInputInt4; pos += 1) { // goes from 0 degrees to desired degrees
    // in steps of 1 degree
     Serial.println("meanwhile otherside servo 1 opening to release plant in number of steps");
  Serial.println(yourInputInt4);
    myservo1.write(pos); 
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  for (pos = yourInputInt4; pos >= 0; pos -= 1) { // goes from desired degrees to 0 degrees
     Serial.println("servo 2 closing to pick plant in number of steps");
  Serial.println(yourInputInt4);
    myservo2.write(pos);              // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);                       // waits 15ms for the servo to reach the position

    
  }
  
/////////////////////////FLIPPNG THE STEPPER 2 START ////////////////////

///////////////////////STEPPER MOTOR 2 PART 2///////////////
  digitalWrite(enable2Pin, HIGH); 
  digitalWrite(DIR2, LOW);
    Serial.println("Stepper motor 2 Spinning Anti-Clockwise...");
  Serial.println("Stepper motor number of steps...");
  Serial.println(yourInputInt2);
  for(int i = 0; i<yourInputInt2; i++)
  {
    digitalWrite(STEP2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP2, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);
digitalWrite(enable2Pin, LOW); 

///////////////////////// LOOP FOR NUMBER OF COLUMNS COUNTING
 myCounter = myCounter + 1;
    lcd.setCursor(0, 0);
     lcd.print("process done ");
     lcd.setCursor(0, 1);
      lcd.print(myCounter);
    Serial.println("process done ");
    Serial.println(myCounter);
}
  else if(digitalRead(mySwitchPin) == HIGH) {
    myCounter = 0;
  }
    lcd.setCursor(0, 0);
    lcd.print("All process done ");
    Serial.println("All process done ");
    delay(1000); 
     lcd.setCursor(0, 1);
    lcd.print("Press Reset to Repeat ");
    Serial.println("Press Reset to Repeat ");
    delay(1000);
}
