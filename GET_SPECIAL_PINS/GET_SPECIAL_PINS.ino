void setup() {
  Serial.begin(115200);
  
  // Identify board type
  Serial.println("Identifying special-purpose pins for the selected board:");
  
  // Print SPI Pins
  Serial.println("\n== SPI Pins ==");
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);  

  // Print I2C Pins
  Serial.println("\n== I2C Pins ==");
  Serial.print("SDA: ");
  #if defined(SDA)
    Serial.println(SDA);
  #else
    Serial.println("N/A");
  #endif
  Serial.print("SCL: ");
  #if defined(SCL)
    Serial.println(SCL);
  #else
    Serial.println("N/A");
  #endif

  // Print UART Pins
  Serial.println("\n== UART Pins ==");
  Serial.print("TX: ");
  #if defined(TX)
    Serial.println(TX);
  #else
    Serial.println("N/A");
  #endif
  Serial.print("RX: ");
  #if defined(RX)
    Serial.println(RX);
  #else
    Serial.println("N/A");
  #endif

  // Print PWM Pins (For ESP32 and ESP8266, we can print some commonly used pins)
  Serial.println("\n== PWM Pins (ESP32/ESP8266) ==");
  #if defined(ESP32) || defined(ESP8266)
    Serial.println("ESP32/ESP8266: PWM available on most digital pins");
  #else
    Serial.println("Arduino: PWM on specific pins (e.g., Uno: 3, 5, 6, 9, 10, 11)");
  #endif
  
  // Print Analog Input Pins
  Serial.println("\n== Analog Input Pins ==");
  #if defined(ESP32)
    Serial.println("ESP32: Pins 0-39 (ADC1: Pins 0-7, ADC2: Pins 8-39)");
  #elif defined(ESP8266)
    Serial.println("ESP8266: Pin A0 (GPIO 17)");
  #elif defined(__AVR_ATmega328P__) // Arduino Uno
    Serial.println("Arduino Uno: Pins A0-A5");
  #elif defined(__AVR_ATmega2560__) // Arduino Mega
    Serial.println("Arduino Mega: Pins A0-A15");
  #else
    Serial.println("Analog input pins not identified for this board.");
  #endif

  // Print Other Special Purpose Pins (like Flash, Touch, etc., for ESP32)
  #if defined(ESP32)
    Serial.println("\n== Other ESP32 Special Pins ==");
    Serial.println("FLASH: GPIO 6-11");
    Serial.println("TOUCH: GPIO 0, 2, 4, 12-15, 27, 32, 33");
    Serial.println("DAC: GPIO 25, 26");
    Serial.println("CAN: GPIO 21, 22");
  #endif
}

void loop() {
  // Your loop code here
}
