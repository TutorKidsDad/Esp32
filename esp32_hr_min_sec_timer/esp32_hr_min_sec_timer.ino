#include <Wire.h>      // Include the I2C library
#include <LiquidCrystal_I2C.h>  // Include the LCD library
#include <WiFiManager.h> // Include WiFiManager library
#include <ESPAsyncWebServer.h> // Include AsyncWebServer library

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Set the LCD address to 0x27 and the number of columns and rows

AsyncWebServer server(80); // Create AsyncWebServer object on port 80

unsigned long duration = 8L * 60L * 60L * 1000L; // Default duration set to 8 hours (in milliseconds)

void setup() {
  lcd.init();      // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  // WiFiManager initialization
  WiFiManager wifiManager;
  wifiManager.autoConnect("Timer-Setup-AP"); // Create an AP named "Timer-Setup-AP" for configuration

  // Define the HTML page to set timer duration
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", R"rawliteral(
      <!DOCTYPE HTML><html>
      <head><title>Set Timer Duration</title></head>
      <body>
        <h1>Set Timer Duration</h1>
        <form action="/setTimer" method="GET">
          <label for="duration">Enter Duration (hours):</label>
          <input type="number" id="duration" name="duration" min="1" max="24" required>
          <input type="submit" value="Set Timer">
        </form>
      </body>
      </html>)rawliteral");
  });

  // Handle setting the timer duration
  server.on("/setTimer", HTTP_GET, [](AsyncWebServerRequest *request){
    if (request->hasParam("duration")) {
      String durationParam = request->getParam("duration")->value();
      int durationHours = durationParam.toInt();
      duration = durationHours * 60L * 60L * 1000L; // Convert duration to milliseconds
      request->send(200, "text/html", "<h1>Timer Set to " + durationParam + " hours</h1><a href='/'>Go Back</a>");
    } else {
      request->send(400, "text/html", "<h1>Duration parameter missing</h1><a href='/'>Go Back</a>");
    }
  });

  server.begin(); // Start the server

  // Start the countdown timer
  unsigned long startTime = millis();  // Get the current time in milliseconds

  // Loop until the timer is finished
  while (millis() - startTime < duration) {
    // Calculate the remaining time
    unsigned long remainingTime = duration - (millis() - startTime);

    // Convert remaining time to hours, minutes, and seconds
    int remainingHours = remainingTime / 3600000;
    int remainingMinutes = (remainingTime % 3600000) / 60000;
    int remainingSeconds = (remainingTime % 60000) / 1000;

    // Display remaining time on LCD
    lcd.clear();  // Clear the LCD screen
    lcd.setCursor(0, 0);  // Set the cursor to the first column of the first row
    lcd.print("Remaining:");  // Display the text "Remaining:"
    lcd.setCursor(0, 1);  // Set the cursor to the first column of the second row
    lcd.print(remainingHours);  // Display the remaining hours
    lcd.print("h ");  // Display the text "h"
    lcd.print(remainingMinutes);  // Display the remaining minutes
    lcd.print("m ");  // Display the text "m"
    lcd.print(remainingSeconds);  // Display the remaining seconds
    lcd.print("s");  // Display the text "s"

    delay(1000);  // Wait for 1 second
  }

  // Display "Timer finished" message on LCD
  lcd.clear();  // Clear the LCD screen
  lcd.setCursor(0, 0);  // Set the cursor to the first column of the first row
  lcd.print("Timer finished");  // Display the text "Timer finished"
}

void loop() {
  // Nothing to do here
}
