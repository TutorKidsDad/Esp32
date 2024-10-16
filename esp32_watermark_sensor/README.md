### Explanation of the Code

This ESP32-based project reads the soil moisture level using a soil moisture sensor and displays the results on an I2C-connected LCD. The code takes multiple readings from the sensor, calculates the resistance of the sensor, and determines the moisture level of the soil. It filters the data using a median filtering technique for noise reduction, and displays the moisture value on both the serial monitor and the LCD.

#### Key Sections:

---

### 1. **Library Inclusions**

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
```

- **`Wire.h`**: Manages I2C communication, required for the I2C-based LCD.
- **`LiquidCrystal_I2C.h`**: Manages communication with the LCD over I2C.

---

### 2. **Constants and Pin Definitions**

```cpp
#define NUM_READS 11
typedef struct { int moisture; long resistance; } values;

const long knownResistor = 4700;
int activeDigitalPin = 18;
int supplyVoltageAnalogPin;
int sensorVoltageAnalogPin;

int supplyVoltage;
int sensorVoltage;
values valueOf[NUM_READS];
```

- **`NUM_READS`**: Defines the number of sensor readings for noise filtering.
- **`activeDigitalPin`**: Alternates between GPIO pins to switch the current flow through the sensor.
- **`knownResistor`**: Represents the known resistance value in the circuit, used to calculate the soil sensor resistance.
- **`supplyVoltageAnalogPin`** and **`sensorVoltageAnalogPin`**: Set during the loop to alternate between ESP32 analog pins (GPIO 34 and 35) for reading voltage.
- **`valueOf`**: Stores pairs of moisture and resistance readings, which will later be filtered.

---

### 3. **LCD Initialization**

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

- Initializes an I2C LCD with address `0x27`, a 16-character, 2-row display. The address can vary depending on the LCD model.

---

### 4. **Setup Function**

```cpp
void setup() {
  Serial.begin(115200);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture");
  
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  delay(200);
}
```

- **Serial.begin(115200)**: Initializes the serial communication at a baud rate of 115200 for debugging.
- **LCD Initialization**: The LCD is initialized and set to display "Soil Moisture" on the first row.
- **Pin Setup**: `pinMode` is used to configure GPIO 18 and 19 as output pins. These pins are alternated for switching the sensor reading current paths.

---

### 5. **Main Loop**

```cpp
void loop() {
  for (int i = 0; i < NUM_READS; i++) {
    setupCurrentPath();
    digitalWrite(activeDigitalPin, HIGH);
    delay(25);
    supplyVoltage = analogRead(supplyVoltageAnalogPin);
    sensorVoltage = analogRead(sensorVoltageAnalogPin);
    digitalWrite(activeDigitalPin, LOW);
    delay(25);

    valueOf[i].resistance = long(float(knownResistor) * (supplyVoltage - sensorVoltage) / sensorVoltage + 0.5);
    valueOf[i].moisture = min(int(pow(valueOf[i].resistance / 331.55, 1.0 / -1.695) * 100 + 0.5), 100);
  }

  sortMoistures();
  
  Serial.print("Resistance: ");
  Serial.print(valueOf[NUM_READS / 2].resistance);
  Serial.print(", Moisture: ");
  Serial.println(valueOf[NUM_READS / 2].moisture);

  lcd.setCursor(0, 1);
  lcd.print("Moisture: ");
  lcd.print(valueOf[NUM_READS / 2].moisture);
  lcd.print("%");

  delay(3000);
}
```

- **Reading Sensor Data**: In the loop, the code reads the sensor voltage multiple times (based on `NUM_READS`), switching between two GPIO pins to ensure accuracy.
  - **`digitalWrite(activeDigitalPin, HIGH)`**: Turns on the voltage supply to the sensor.
  - **`analogRead(supplyVoltageAnalogPin)`** and **`analogRead(sensorVoltageAnalogPin)`**: Read voltage values from the sensor.
  - **Resistance Calculation**: The resistance of the soil sensor is calculated using the known resistor and the voltage difference.
  - **Moisture Calculation**: The moisture level is computed based on the resistance using a nonlinear equation derived from sensor characteristics.

- **Sorting and Filtering**: The readings are sorted to apply median filtering (removing noise).
  
- **Display and Serial Output**: After sorting, the median value of the moisture reading is displayed on the LCD and printed to the serial monitor.

---

### 6. **Switching the Current Path (`setupCurrentPath`)**

```cpp
void setupCurrentPath() {
  if (activeDigitalPin == 18) {
    activeDigitalPin = 19;
    supplyVoltageAnalogPin = 34;
    sensorVoltageAnalogPin = 35;
  } else {
    activeDigitalPin = 18;
    supplyVoltageAnalogPin = 35;
    sensorVoltageAnalogPin = 34;
  }
}
```

This function alternates the GPIO pins used for sensor measurement between GPIO 18 and GPIO 19. It switches the pins used for reading voltage and ensures that readings are accurate by using both sides of the sensor.

---

### 7. **Sorting Moisture Readings (`sortMoistures`)**

```cpp
void sortMoistures() {
  int j;
  values temp;
  for (int i = 0; i < NUM_READS - 1; i++) {
    for (int j = i + 1; j < NUM_READS; j++) {
      if (valueOf[i].moisture > valueOf[j].moisture) {
        temp = valueOf[i];
        valueOf[i] = valueOf[j];
        valueOf[j] = temp;
      }
    }
  }
}
```

This function implements a simple selection sort algorithm to sort the moisture readings in ascending order. It ensures that the data is filtered and the median value is used to avoid noise or erratic sensor behavior.

---

### Summary:

- **Functionality**: The code reads the soil moisture sensor multiple times, calculates the resistance of the soil based on sensor readings, filters the data using a median filter, and displays the moisture percentage on both the serial monitor and the I2C LCD.
- **I2C LCD**: The use of I2C reduces the number of pins required for the LCD, making it efficient for ESP32.
- **Pin Swapping**: The code alternates between two GPIO pins to supply voltage to the sensor and take accurate readings.

This setup ensures stable and accurate moisture readings, suitable for monitoring soil in gardening or agricultural applications.
