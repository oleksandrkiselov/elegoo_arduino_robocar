# Lecture 2 Summary

**Topic:** Mecanum Wheel Robot Car: Mechanics, Electronics, and Programming
**Instructor:** Viktor Rovinskyi
**Date:** January 29, 2026
**Duration:** ~2 hours

---

## 1. Mecanum Wheels — Construction and Principles

Mecanum wheels have rollers mounted at 45° to the rotation axis, enabling omnidirectional movement without turning the chassis.

### Vector Force Addition
Each wheel transmits force through angled rollers. The force splits into:
- Longitudinal component (along wheel direction)
- Lateral component (perpendicular)

By combining rotation of 4 wheels, lateral forces cancel and longitudinal forces add up — resulting in movement in any direction.

### Basic Movement Combinations

| Movement | Wheel Configuration |
|----------|---------------------|
| Forward | All 4 wheels forward |
| Backward | All 4 wheels backward |
| Left (strafe) | Left wheels backward, right wheels forward |
| Right (strafe) | Mirror of left |
| Diagonal left-forward | Two wheels on one diagonal forward |
| Diagonal right-forward | Two wheels on other diagonal forward |
| Rotate CW | Left wheels forward, right wheels backward |
| Rotate CCW | Left wheels backward, right wheels forward |

### Limitations
**Critical:** Mecanum wheels only work on smooth, flat surfaces (concrete, linoleum). They fail on uneven, soft, or carpeted surfaces. Industrial use: warehouses and aircraft hangars with level floors.

---

## 2. Hardware Components

| Component | Function |
|-----------|----------|
| Arduino Uno (ATmega328P) | Main microcontroller |
| Elegoo SmartCar Shield V1.1 | Expansion board with connectors |
| TB6612 Motor Driver | Controls 4 DC motors |
| 4× DC motors with gearboxes | Wheel drives |
| Ultrasonic sensor (HC-SR04) | Distance measurement |
| IR receiver | Remote control |
| Servo motor | Rotates ultrasonic sensor |
| Line tracking module | Follows lines on floor |
| ESP32 (UART) | WiFi/Bluetooth communication |

### Upload/CAM Switch
- **Upload position:** TX/RX disconnected from ESP32 — allows firmware upload via USB
- **CAM position:** Working mode — ESP32 connected to Arduino for data exchange

---

## 3. Motor Types

### DC Brushed Motor
Simplest DC motor. Rotor spins via magnetic field interaction. Current flows through brushes sliding on commutator (copper plates).
- **Drawback:** Sparking during commutation creates electromagnetic interference (parasitic radio transmitter)
- Brushes wear over time

### Brushless Motor (BLDC)
No mechanical contact between rotor and stator. Uses Hall sensors to detect rotor position. Transistors switch windings accordingly.
- **Advantages:** Longer lifespan, less interference, higher efficiency, up to 100,000 RPM unloaded
- **Disadvantage:** Higher cost due to built-in electronics
- Used in drones and cooling fans

### Gearbox (Reducer)
Converts high RPM / low torque → low RPM / high torque (like electrical transformer).
Gear ratio determined by teeth count ratio. Our motors have built-in gearboxes.

---

## 4. Motor Driver — TB6612

### Why a Driver is Needed
Arduino outputs only ~40mA at 5V — insufficient for motors (hundreds of mA).
Also, reversing motor requires polarity change, which Arduino can't do directly.

**Driver solves both:** amplifies current and implements H-bridge for reversal.

### TB6612FNG Specifications
- 4 independent H-bridges (one per motor)
- Max motor voltage: 30V (we use 6–7.4V)
- Peak current per channel: 1.2A
- Inputs: Enable (STBY), IN1, IN2 per channel
- Supports PWM for speed control

### Direction Control Logic

| IN1 | IN2 | Result |
|-----|-----|--------|
| 1 | 0 | Forward |
| 0 | 1 | Backward |
| 1 | 1 | Brake |
| 0 | 0 | Stop |

For 4 motors: 8 control lines needed → shift register used due to limited Arduino pins.

---

## 5. Digital Fundamentals

### Binary System
Computers use only 0 (low, ~0V) and 1 (high, ~5V).
- **Byte:** 8 bits, stores 0–255 (2⁸ = 256 values)
- **Hex:** Each digit = 4 bits, byte = 2 hex digits (e.g., 0xFF = 11111111₂ = 255)

### Shift Register
Expands 2–3 Arduino pins to 8+ outputs.
- DATA line sends bits sequentially with CLOCK signal
- LATCH signal locks values to outputs simultaneously
- Noise protection: register open for writing only during LATCH pulse

### Logic Gates

| Gate | Function | Analogy |
|------|----------|---------|
| AND | Output=1 only if both inputs=1 | Two switches in series |
| OR | Output=1 if any input=1 | Two switches in parallel |
| NOT | Inverts input | |
| XOR | Output=1 if inputs differ | |

**OR operation** combines bits: to set multiple bits in a byte, use bitwise OR between them.

### PWM (Pulse Width Modulation)
Controls motor speed without changing voltage. Duty cycle determines average voltage:
- 0 = stopped
- 255 = max speed (8-bit PWM)

Our board has 2 PWM channels: left wheels and right wheels.

### Frequency and Period
- Frequency (f) = oscillations per second (Hz)
- Period (T) = duration of one cycle
- f = 1/T

Arduino Uno: 16 MHz clock, divided by counters for PWM and delays.

---

## 6. Arduino Uno — ATmega328P

### Architecture
ATmega328P is an 8-bit AVR microcontroller. Arduino Uno wraps it with USB interface, connectors, and voltage regulator.

**Pin naming:** Internal names (PD0, PB0...) differ from Arduino names (0, 1, A0...). Code uses Arduino names or defined constants.

### Registers
Registers are special memory bytes controlling all peripherals (timers, UART, SPI, etc.). Each bit has a specific function. Full documentation: ATmega328P datasheet (~300 pages).

Arduino IDE hides complexity via library functions (digitalWrite, analogWrite), but direct register access is sometimes more efficient.

---

## 7. Programming Movement

### Program Structure

```
shiftOut()      → Sends 8 bits to shift register
motor()         → Takes direction byte + PWM values, calls shiftOut and analogWrite
analogWrite()   → Sets wheel speed (0–255)
digitalWrite()  → Enables/disables TB6612 via STBY
```

### Direction Encoding
Each wheel: 2 bits (forward/backward). All 8 bits of one byte describe 4 wheels.
- Forward: 4 "forward" bits = 1, rest = 0
- Example: 0x5A or 92 decimal (depends on wiring)

### Wheel Calibration Procedure

1. Comment out all movement except one (e.g., "forward")
2. Test one bit at a time (1, 2, 4, 8, 16, 32, 64, 128)
3. Observe which wheel moves and in which direction
4. Map bits to wheel names (left-front-forward, etc.)
5. Build correct mapping table
6. Verify robot drives straight with all 4 wheels forward
7. Uncomment full program and test all directions

### Safety Notes
- Charge battery before running
- **Never run on table without securing** — robot is fast and will fall
- Check polarity before connecting battery
- If motors don't spin — check screw terminal contacts

---

## 8. Additional Concepts

### Amplifier vs. Reducer/Transformer
- **Reducer/Transformer:** Redistributes power between speed/torque or voltage/current. Output power ≤ input (efficiency < 100%)
- **Amplifier:** Small input power controls large output power from separate source (transistor, audio amp, hydraulic steering)

### Hall Sensor
Semiconductor detecting magnetic field. Current flows through plate; magnetic field deflects electrons (Lorentz force), creating voltage proportional to field strength. Used in BLDC motors and current sensors.

### Harmonic Drive
Special gearbox for robotics. Flexible gear wheel rotates inside rigid ring. Large reduction ratio in small size, no backlash. Input and output axes are coaxial.
