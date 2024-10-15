This code logs environmental data (temperature, humidity, and CO2 levels) to a CSV file on an SD card using a data logger shield. It interacts with a CO2 sensor (COZIR), a real-time clock (DS1307), and an SD card to store the logged data. Here's a breakdown of what each part of the code is doing:

### Key Components:
- **Libraries:**
  - `SD.h`: Handles reading and writing to the SD card.
  - `Wire.h`: For I2C communication (used by the real-time clock).
  - `TimeLib.h` and `DS1307RTC.h`: For retrieving and working with the current time from the DS1307 RTC.
  - `SoftwareSerial.h`: For communication with the COZIR sensor over a serial connection.
  - `cozir.h`: A library to interact with the COZIR CO2 sensor, which also provides temperature and humidity readings.

### Main Workflow:
1. **Setup Function:**
   - Initializes serial communication at 9600 baud.
   - Sets up the SD card by checking if it's present and can be initialized.
   - Initializes communication with the COZIR sensor over software serial (pins 2 and 3).
   - Waits 5 seconds to allow the sensor to start up.

2. **Loop Function:**
   - The loop continuously polls data from the COZIR sensor and logs it to the SD card at regular intervals.
   
   - **Polling Data:**
     - Polls the sensor data (temperature, humidity, CO2) every minute and stores the lowest, average, and highest values over that time.
     - This ensures that even extreme values within the minute are captured without overwhelming the log with too much data.
   
   - **Logging Data:**
     - Every 15 minutes (on the 0th, 15th, 30th, and 45th minute), it writes the polled data (low, average, and high values) for temperature, humidity, and CO2 to the SD card as a new row in a CSV file.
     - A timestamp is included with each log entry using the RTC.

3. **Functions:**
   - **`pollData()`**: Collects the data from the COZIR sensor and updates the arrays for temperature, humidity, and CO2. It tracks the lowest, average, and highest values during the current interval.
   - **`logData()`**: Opens the CSV file, checks if it has a header, and writes the data collected over the 15-minute interval to the file.
   - **`writeToSD()`**: Handles the actual writing of the data to the CSV file, including formatting the timestamp and the environmental data.
   - **`resetPolls()`**: Resets the stored data arrays and the poll counter for the next logging interval.
   - **`addTableHeader()`**: Adds a header row to the CSV file if it's missing.
   - **`getFirstWord()`**: Reads the first few characters of the CSV file to check if it's empty or correctly formatted.
   - **`twoDigits()`**: Formats single-digit numbers with a leading zero for the timestamp.

### What it Accomplishes:
- **Polling Sensor Data**: The COZIR sensor provides temperature, humidity, and CO2 readings at frequent intervals (every minute). The code captures these values while monitoring extremes.
  
- **Logging to SD Card**: Every 15 minutes, the code logs the collected environmental data to a CSV file. The CSV contains the time of logging and the lowest, average, and highest readings for each parameter during the 15-minute period.

- **Time Management**: The code uses the real-time clock (RTC) to timestamp the logged data, ensuring accurate logging intervals.

### Example of CSV Output:
The CSV file contains rows with the following format:
```
Time,Temperature (low),Temperature,Temperature (high),Humidity (low),Humidity,Humidity (high),CO2 (low),CO2,CO2 (high),Measurements
2024-10-15 14:15:00,20.1,21.5,23.3,30.2,35.6,40.1,400.0,415.0,430.0,10
```
This row shows:
- A log taken at 14:15:00 on October 15, 2024.
- The temperature ranged from 20.1°C to 23.3°C, with an average of 21.5°C.
- Humidity ranged from 30.2% to 40.1%, with an average of 35.6%.
- CO2 levels ranged from 400 ppm to 430 ppm, with an average of 415 ppm.
- 10 polls were taken during this 15-minute period.

### In Summary:
This code is designed to regularly sample environmental conditions (temperature, humidity, CO2) and log this data to an SD card with accurate timestamps using a real-time clock. It's efficient in that it only logs data every 15 minutes but captures sensor data every minute, ensuring important fluctuations are not missed.
