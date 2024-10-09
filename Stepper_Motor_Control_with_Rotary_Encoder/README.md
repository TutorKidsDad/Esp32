### Stepper Motor Control with Rotary Encoder and I2C LCD Display

This project demonstrates controlling a stepper motor using a rotary encoder for speed adjustment, a push button for changing the direction, and an I2C LCD display to show the motor's speed and direction.

---

#### Components:
1. **Stepper Motor**: Connected to `DIR` and `STEP` pins, the motor is controlled by changing the direction and speed.
2. **Rotary Encoder**: Used to adjust the speed of the stepper motor. Each turn of the encoder increases or decreases the motor speed.
3. **Push Button (or Encoder Button)**: A button that toggles the motor’s direction between clockwise and counterclockwise.
4. **I2C LCD Display**: Displays the current speed and direction of the stepper motor in real-time.

#### Pin Connections:
- **Stepper Motor**: 
  - `DIR` (Direction Pin): Pin 12
  - `STEP` (Step Pin): Pin 14
- **Rotary Encoder**:
  - `Pin A`: Pin 2
  - `Pin B`: Pin 3
  - **Push Button**: Pin 4 (Can be from the rotary encoder itself)
- **LM35 Temperature Sensor**: (Optional: Can be added for temperature monitoring)
- **I2C LCD**: Address `0x27`, connected via the I2C protocol (SDA, SCL lines).

---

#### Features:

1. **Motor Speed Control**:
   - The rotary encoder changes the speed of the stepper motor. When the encoder is rotated, the motor speed is adjusted by modifying the delay between pulses sent to the motor.
   - The speed is represented by a delay (in microseconds) between step pulses. A lower delay means a faster motor speed.
   - The speed value is shown on the I2C LCD in real-time, and it ranges from a minimum of 200 µs to a maximum of 5000 µs.

2. **Direction Control**:
   - The motor direction can be toggled by pressing the push button connected to the rotary encoder.
   - If the button is pressed, the motor direction switches between clockwise and counterclockwise. This change is displayed on the LCD.

3. **LCD Display**:
   - The I2C LCD shows two key pieces of information:
     - **Motor Speed**: Displayed as the pulse delay in microseconds (µs).
     - **Motor Direction**: Whether the motor is spinning clockwise or counterclockwise.
   - The LCD updates the motor speed dynamically as the rotary encoder is turned.
   - The direction is updated when the button is pressed, indicating the current motor rotation state.

---

#### How It Works:

1. **Rotary Encoder for Speed Control**:
   - The rotary encoder is monitored continuously. As the user rotates the encoder, the code checks for changes in its position.
   - If the encoder is turned clockwise, the motor speed increases (delay between steps decreases).
   - If turned counterclockwise, the motor speed decreases (delay increases).
   - The motor’s speed is adjusted by modifying the delay between the `STEP` pulses, controlling how fast the motor moves.

2. **Button for Direction Control**:
   - The button connected to the encoder (or a separate button) toggles the direction of the motor when pressed.
   - Button press detection uses a debouncing technique to ensure that false triggers do not occur due to signal noise.
   - When the button is pressed, the direction pin (`DIR`) is toggled between HIGH and LOW, changing the motor’s rotation from clockwise to counterclockwise or vice versa.

3. **I2C LCD Display**:
   - The 16x2 LCD provides real-time feedback to the user about the current motor speed and direction.
   - It helps the user monitor the system without relying on serial output, making it a more user-friendly and portable solution.

---

#### Code Overview:

- **Initialization**:
  - The stepper motor control pins (`STEP`, `DIR`), rotary encoder pins (`A`, `B`), and the button pin are initialized.
  - The I2C LCD is set up to display two lines of information: speed and direction.

- **Speed Adjustment**:
  - The rotary encoder changes the motor speed by adjusting the delay between step pulses.
  - The LCD is updated dynamically to show the current speed.

- **Direction Control**:
  - The button toggles the direction, and this is reflected both in the motor’s movement and on the LCD.

---

### Circuit:

| Component           | Pin         | Description                         |
|---------------------|-------------|-------------------------------------|
| Stepper Motor DIR    | Pin 12      | Direction control for the motor     |
| Stepper Motor STEP   | Pin 14      | Step control for the motor          |
| Rotary Encoder Pin A | Pin 2       | Rotary encoder signal A             |
| Rotary Encoder Pin B | Pin 3       | Rotary encoder signal B             |
| Push Button          | Pin 4       | Button for toggling motor direction |
| I2C LCD (SDA)        | A4          | I2C data line for the LCD           |
| I2C LCD (SCL)        | A5          | I2C clock line for the LCD          |

---

### Conclusion:

This project showcases how to control a stepper motor's speed and direction using a rotary encoder and a push button, with real-time feedback displayed on an I2C LCD. It offers a simple but effective interface for motor control, making it ideal for projects requiring precision movement and user-friendly control mechanisms.
