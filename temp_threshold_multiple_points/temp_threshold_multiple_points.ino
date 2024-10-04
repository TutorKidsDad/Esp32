#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Board-specific definitions
#if defined(ESP8266)
  #define ONE_WIRE_BUS D4
  #define SDA_PIN D2
  #define SCL_PIN D1
  #define BUZZER_PIN D3
#elif defined(ESP32)
  #define ONE_WIRE_BUS 4
  #define SDA_PIN 21
  #define SCL_PIN 22
  #define BUZZER_PIN 17
#elif defined(ARDUINO_UNO)
  #define ONE_WIRE_BUS 4
  #define SDA_PIN A4
  #define SCL_PIN A5
  #define BUZZER_PIN 3
#else
  #error "Unsupported board! Define the correct pins for your board."
#endif

// Thresholds
const int THRESHOLD_1 = 70; // °C
const int THRESHOLD_2 = 80; // °C
const int THRESHOLD_3 = 90; // °C
const int THRESHOLD_4 = 100; // °C

// Other constants
const unsigned long BUZZER_INTERVAL = 500; // Time between buzzer beeps
#define TEMP_SIZE 7

// Global variables
char temperatureCString[TEMP_SIZE];
char previousTemp[TEMP_SIZE] = "init";
unsigned long previousMillis = 0; // For non-blocking delay
float currentTemperature;

// Setup OneWire and DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

// Initialize the LCD with the I2C address
LiquidCrystal_I2C lcd(0x27, 16, 2);

enum AlarmState { IDLE, ALERT_STAGE_1, ALERT_STAGE_2, ALERT_STAGE_3, ALERT_STAGE_4 };
AlarmState currentState = IDLE;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  
  Serial.begin(9600);
  Wire.begin(SDA_PIN, SCL_PIN); // I2C initialization
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Temperature");
  lcd.setCursor(3, 1);
  lcd.print("Reading....");
  delay(2000);

  DS18B20.begin(); // Initialize temperature sensor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp.:");
}

float getTemperature() {
  DS18B20.requestTemperatures();
  float tempC = DS18B20.getTempCByIndex(0);
  dtostrf(tempC, 3, 1, temperatureCString); // Convert temperature to string
  
  if (tempC == -127.0) { // Error handling for disconnected sensor
    Serial.println("Error: Sensor disconnected");
    return -127.0;
  }

  return tempC;
}

void updateLCD() {
  if (strcmp(previousTemp, temperatureCString) != 0) {
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print(temperatureCString);
    strncpy(previousTemp, temperatureCString, TEMP_SIZE);
  }
}

void triggerBuzzer(unsigned int duration) {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= BUZZER_INTERVAL) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    previousMillis = currentMillis;
  }
}

void checkTemperatureAndUpdateState() {
  currentTemperature = getTemperature();
  
  if (currentTemperature < THRESHOLD_1) {
    currentState = IDLE;
  } else if (currentTemperature >= THRESHOLD_1 && currentTemperature < THRESHOLD_2) {
    currentState = ALERT_STAGE_1;
  } else if (currentTemperature >= THRESHOLD_2 && currentTemperature < THRESHOLD_3) {
    currentState = ALERT_STAGE_2;
  } else if (currentTemperature >= THRESHOLD_3 && currentTemperature < THRESHOLD_4) {
    currentState = ALERT_STAGE_3;
  } else {
    currentState = ALERT_STAGE_4;
  }
}

void handleState() {
  switch (currentState) {
    case IDLE:
      Serial.println("Heating...Wait");
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Heating...Wait");
      break;

    case ALERT_STAGE_1:
      Serial.println("Alarm Stage 1");
      lcd.clear();
      lcd.print("Stage-1 70-80");
      lcd.setCursor(2, 1);
      lcd.print(temperatureCString);
      triggerBuzzer(200);
      break;

    case ALERT_STAGE_2:
      Serial.println("Alarm Stage 2");
      lcd.clear();
      lcd.print("Stage-2 80-90");
      lcd.setCursor(2, 1);
      lcd.print(temperatureCString);
      triggerBuzzer(300);
      break;

    case ALERT_STAGE_3:
      Serial.println("Alarm Stage 3");
      lcd.clear();
      lcd.print("Stage-3 90-100");
      lcd.setCursor(2, 1);
      lcd.print(temperatureCString);
      triggerBuzzer(400);
      break;

    case ALERT_STAGE_4:
      Serial.println("Alarm Stage 4");
      lcd.clear();
      lcd.print("Stage-4 T>100");
      lcd.setCursor(2, 1);
      lcd.print(temperatureCString);
      triggerBuzzer(500);
      break;

    default:
      Serial.println("No Alarm");
      digitalWrite(BUZZER_PIN, LOW);
      break;
  }
}

void loop() {
  // Get current temperature and update the LCD display
  getTemperature();
  updateLCD();
  
  // Check the current temperature and set the alarm state
  checkTemperatureAndUpdateState();
  
  // Handle the current alarm state (idle, stage 1-4)
  handleState();
  
  // Non-blocking delay in main loop
  delay(1000);
}
