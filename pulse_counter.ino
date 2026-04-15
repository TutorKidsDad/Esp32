// ----------------------------
// ESP32 RELAY + FLOW TEST MODE
// ----------------------------

int relayPin = 25;       // change if needed
volatile int pulseCount = 0;
float totalML = 0;

unsigned long lastCalc = 0;
unsigned long pulseSimTimer = 0;

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  Serial.println("\n==============================");
  Serial.println("   ESP32 TEST MODE STARTED");
  Serial.println("==============================\n");

  testWebDashboard();
}

void loop() {

  simulateFlowPulses();     // creates fake pulses every 100ms
  calculateFlow();          // prints flow rate
  test250mlAutoStop();      // stops at 250ml
}

// ---------------------------------------------------------
// 1) WEB DASHBOARD TEST MODE PRINTS
// ---------------------------------------------------------
void testWebDashboard() {
  Serial.println("=== WEB DASHBOARD TEST MODE ===");
  Serial.println("WiFi Connected! (Simulated)");
  Serial.println("IP Address: 192.168.4.1 (Simulated)");
  Serial.println("Dashboard Ready.");
  Serial.println("==============================\n");
}

// ---------------------------------------------------------
// 2) SIMULATE FLOW METER PULSES (NO SENSOR REQUIRED)
// ---------------------------------------------------------
void simulateFlowPulses() {
  if (millis() - pulseSimTimer >= 100) {  
    pulseSimTimer = millis();

    pulseCount += 5;   // add fake pulses
    totalML += 2.5;    // assume roughly 2.5ml per cycle

    Serial.print("[SIM] Pulses Added: ");
    Serial.print(pulseCount);
    Serial.print(" | Total mL: ");
    Serial.println(totalML);
  }
}

// ---------------------------------------------------------
// 3) FLOW RATE CALCULATION (SERIAL ONLY)
// ---------------------------------------------------------
void calculateFlow() {
  if (millis() - lastCalc >= 1000) {
    lastCalc = millis();

    float frequency = pulseCount;      // fake Hz
    float flowRate = frequency / 7.5;  // typical YF-S201 formula

    Serial.println("\n=== FLOW RATE CALCULATION ===");
    Serial.print("Pulses This Second: ");
    Serial.println(pulseCount);
    Serial.print("Frequency (Hz): ");
    Serial.println(frequency);
    Serial.print("Flow Rate (L/min): ");
    Serial.println(flowRate, 2);
    Serial.println("=============================\n");

    pulseCount = 0;
  }
}

// ---------------------------------------------------------
// 4) 250 ml AUTO-STOP TEST
// ---------------------------------------------------------
void test250mlAutoStop() {
  if (totalML >= 250) {
    Serial.println("=== 250ML AUTO-STOP TEST ===");
    Serial.println("Reached 250 ml!");
    Serial.println("Relay OFF (Pump Stop).");
    Serial.println("=============================\n");

    digitalWrite(relayPin, LOW);
    while (1); // freeze program after stop
  } else {
    digitalWrite(relayPin, HIGH); // keep pump ON during filling
  }
}
