
# MQ-135 IoT Air Monitor (ESP32)

## Overview

ESP32-based smart air quality monitor with WiFi and advanced ADC resolution.

---

## Components Required

* ESP32 Dev Module
* MQ-135 Sensor Module
* Breadboard
* Jumper wires

---

## Wiring Connections

| MQ-135 | ESP32        |
| ------ | ------------ |
| VCC    | 5V           |
| GND    | GND          |
| A0     | GPIO34 (ADC) |
| D0     | Not required |

---

## Important Voltage Warning

ESP32 ADC max input = **3.3V**

If MQ-135 outputs 5V → use voltage divider:

Example divider:

* 10kΩ resistor
* 20kΩ resistor

---

## ADC Details

* ESP32 ADC range = 0–4095 (12-bit)
* More accurate than Arduino/ESP8266
* Use `analogReadResolution(12);`

---

## Typical ESP32 Values

| Condition | ADC Value |
| --------- | --------- |
| Clean Air | 800–1500  |
| Moderate  | 1500–2500 |
| Poor      | 2500+     |

---

## IoT Capabilities

* Web server hosting
* MQTT support
* ThingSpeak integration
* Mobile app dashboards

---

## Calibration Steps

1. Burn-in 24–48 hours
2. Warm-up 5–10 minutes
3. Measure clean air average
4. Set thresholds 20–30% above baseline

---

## Notes

* ESP32 ADC is sensitive to noise
* Avoid using WiFi + ADC on same pin groups if unstable
* Use averaging for accuracy

---

# Common Technical Notes (All Boards)

✔ Sensor heater consumes ~150mA
✔ Works best at room temperature
✔ Humidity affects readings
✔ Use moving average filter for stable data
✔ Not industrial-grade certified

---

