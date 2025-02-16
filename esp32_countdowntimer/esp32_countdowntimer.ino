#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h> // Include WiFiManager library
#include <ESPAsyncWebServer.h> // Include AsyncWebServer library

#define BUZZER_PIN 23   // Buzzer pin
#define UP_BUTTON 17    // Up button pin
#define DOWN_BUTTON 18  // Down button pin
#define START_BUTTON 19  // Start button pin

int timerValue = 10;   // Default timer value in seconds
bool timerRunning = false;
bool timerFinished = false;
int countdownValue = 0; // Current countdown value in seconds

LiquidCrystal_I2C lcd(0x27, 16, 2);
AsyncWebServer server(80); // Create AsyncWebServer object on port 80

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();
  lcd.print(" Countdown Timer");
  lcd.setCursor(0, 1);
  lcd.print("   Press start");

  // WiFiManager initialization
  WiFiManager wifiManager;
  wifiManager.autoConnect("Timer-Setup-AP"); // Create AP with this name

  // Define the HTML page to take timer input
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE HTML><html>
      <head><title>Set Timer</title></head>
      <body>
        <h1>Set Countdown Timer</h1>
        <form action="/setTimer" method="GET">
          <label for="time">Enter Time (seconds):</label>
          <input type="number" id="time" name="time" min="10" required>
          <input type="submit" value="Set Timer">
        </form>
      </body>
      </html>)rawliteral");
  });

  // Handle setting the timer value
  server.on("/setTimer", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("time")) {
      String time = request->getParam("time")->value();
      timerValue = time.toInt();
      request->send(200, "text/html", "<h1>Timer Set to " + time + " seconds</h1><a href='/'>Go Back</a>");
    } else {
      request->send(400, "text/html", "<h1>Time parameter missing</h1><a href='/'>Go Back</a>");
    }
  });

  server.begin(); // Start the server
}

void loop() {
  if (!timerRunning) {
    // Check if the up button is pressed and increase the timer value
    if (digitalRead(UP_BUTTON) == LOW) {
      timerValue += 10;
      lcd.clear();
      lcd.print(" Countdown Timer");
      lcd.setCursor(0, 1);
      lcd.print("   Set time: ");
      lcd.print(timerValue);
      delay(200);
    }

    // Check if the down button is pressed and decrease the timer value
    if (digitalRead(DOWN_BUTTON) == LOW) {
      timerValue -= 10;
      if (timerValue < 10) timerValue = 10;
      lcd.clear();
      lcd.print(" Countdown Timer");
      lcd.setCursor(0, 1);
      lcd.print("   Set time: ");
      lcd.print(timerValue);
      delay(200);
    }

    // Check if the start button is pressed and start the countdown
    if (digitalRead(START_BUTTON) == LOW) {
      timerRunning = true;
      countdownValue = timerValue;
      lcd.clear();
      lcd.print(" Countdown Timer");
      lcd.setCursor(0, 1);
      lcd.print("  Time left: ");
      lcd.print(countdownValue);
      delay(200);
    }
  }

  // Countdown loop
  if (timerRunning) {
    if (countdownValue > 0) {
      countdownValue--;
      lcd.setCursor(12, 1);
      lcd.print(countdownValue);
      delay(1000);
    } else {
      // Timer finished, sound the buzzer
      timerRunning = false;
      timerFinished = true;
      digitalWrite(BUZZER_PIN, HIGH);
      lcd.clear();
      lcd.print(" Countdown Timer");
      lcd.setCursor(0, 1);
      lcd.print("     Time up!");
      delay(2000);
      digitalWrite(BUZZER_PIN, LOW);
    }
  }
}
