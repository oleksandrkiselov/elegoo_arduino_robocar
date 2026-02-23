# Lecture 3 Summary

**Topic:** ESP32, Web Camera, and Microcontroller Architecture
**Instructor:** Viktor Rovinskyi

---

## 1. ESP32 Setup in Arduino IDE

### Installation
1. Open Board Manager in Arduino IDE
2. Find `esp32` package by Espressif Systems
3. **IMPORTANT:** Install version **2.0.17** (latest v2 branch)
   - Do NOT update to v3 — may not support older firmware
4. Add ESP32 board manager URL to Arduino IDE settings

### Board Selection
After installation: **Tools → Board → ESP32 → AI Thinker ESP32-CAM**

### Recommended Compile Settings
| Setting | Value |
|---------|-------|
| CPU Frequency | 240 MHz |
| Flash Frequency | 80 MHz |
| Partition Scheme | Default 4MB with SPIFFS (3MB App / 1MB SPIFFS) |

**Note:** ESP32 compiles significantly slower than Arduino due to larger codebase.

---

## 2. Programming ESP32 — Critical Steps

### Disconnect TX/RX Lines Before Programming
ESP32 and Arduino communicate via serial port (TX/RX). During ESP32 programming:
- These lines must be disconnected
- Arduino on same lines interferes with flashing
- Reconnect after programming for normal operation
- **This is the most common cause of "not working" after flashing**

### After Flashing
ESP32 often doesn't auto-start. Solutions:
1. Press **Reset (RST)** button — use pencil/matchstick (narrow access)
2. If button doesn't work — disconnect power for 15 seconds, reconnect

### WiFi Configuration
Change SSID and password in code to your network.
- Phone hotspot may have limitations
- USB modem may have NAT/filtering conflicts
- **Recommended:** Use regular home WiFi router

---

## 3. Web Interface for Car Control

### Video Stream
ESP32-CAM operation cycle:
1. Capture frame from camera
2. Compress to JPEG
3. Transmit over network (streaming mode)

Settings:
- Resolution: 480×320 (optimal quality/speed balance)
- JPEG quality: 0–63 (lower = better quality, larger file)

### Fixing Inverted Image
If image is upside-down: find `rotation` parameter in code, set to `180` instead of `0`.

### Command Protocol
Commands sent via serial port, 1 byte each. Format for reliability:
- **Prefix:** 0x55 (binary: 01010101)
- **Suffix:** 0xAA (binary: 10101010) — inverted prefix
- **Command:** 1 byte between prefix and suffix

This structure distinguishes real commands from serial port noise.

### Available Commands
`LEFT`, `RIGHT`, `FORWARD`, `BACKWARD`, `STOP`, `LIGHT ON/OFF`

### Adding Custom Buttons
Web interface is HTML string in ESP32 code:
1. Copy existing button block
2. Change command value (byte between prefix/suffix)
3. Set style, color, size via CSS attributes
4. `onmousedown`/`onmouseup` handlers send command on press, STOP on release

### Movement Parameters
Command codes must match settings from previous lab (left/right side codes, motor speed and direction). Mismatch = wrong movement direction.

---

## 4. ESP32 vs Arduino Comparison

### Arduino Uno (ATmega328P)
| Spec | Value |
|------|-------|
| Processor | 8-bit |
| Clock | 16 MHz |
| Flash | 32 KB (~2 KB bootloader, ~30 KB program) |
| RAM | 2 KB |
| EEPROM | 1 KB |

### ESP32-CAM
| Spec | Value |
|------|-------|
| Processor | 32-bit, 2 cores (Xtensa LX6) |
| Clock | Up to 240 MHz (15–20× faster) |
| Connectivity | Built-in WiFi + Bluetooth |
| Camera | Built-in (OV2640 or OV7670) |
| GPIO | More pins, more serial ports |
| Memory | Significantly larger Flash & RAM |

---

## 5. Microcontroller Architecture (ATmega328P)

### Microcontroller vs Microprocessor
- **Microprocessor system (desktop PC):** Separate components (motherboard, CPU, RAM, BIOS, ports, expansion cards)
- **Microcontroller:** All components in one chip (CPU, RAM, Flash, EEPROM, clock, ports, timers, ADC)

### Memory Types

| Type | Size | Purpose | Notes |
|------|------|---------|-------|
| Flash | 32 KB | Program storage | Written during flashing, limited write cycles (100K–1M) |
| RAM | 2 KB | Runtime variables | Cleared on reset |
| EEPROM | 1 KB | Settings storage | Persists between restarts, limited writes — avoid cyclic writing |

### Bootloader
Arduino = ATmega328P pre-programmed with bootloader (~2 KB Flash).
Bootloader enables USB/UART programming without external programmer — this made Arduino accessible to everyone.

---

## 6. Clock Generator and Crystal Resonator

Every microprocessor needs a clock generator producing uniform pulses. Speed depends on pulse frequency.

**Crystal resonator:** Piezoelectric quartz crystal (SiO₂) with stable resonant frequency, temperature-independent. Capacitors in circuit prevent unwanted low-frequency oscillation.

Microcontroller can work without external crystal (internal RC oscillator), but frequency will drift with temperature.

---

## 7. Timers and Counters

### ATmega328P Timers
| Timer | Bits | Range |
|-------|------|-------|
| Timer0 | 8-bit | 0–255 |
| Timer1 | 16-bit | 0–65535 |
| Timer2 | 8-bit | 0–255 |

Max value of N-bit counter = 2^N − 1

### Counter vs Timer Mode
- **Counter:** Counts external pulses on T0/T1 pins
- **Timer:** Counts internal clock pulses → measures time intervals

### Prescaler
Divides input clock by powers of 2: /1, /8, /64, /256, /1024
- Enables slower timers for longer intervals
- Example: 16 MHz / 1024 ≈ 15625 Hz → 16-bit timer fills in ~4 seconds

### PWM Generation
Timers generate PWM for power control (motor speed).
`analogWrite()` uses timers for PWM. Only 3 timers = limited hardware PWM outputs; more = software PWM (less accurate).

---

## 8. ADC — Analog-to-Digital Converter

ATmega328P has 6 analog inputs (A0–A5), but only ONE ADC with multiplexer (input switcher).

### Specifications
| Parameter | Value |
|-----------|-------|
| Input voltage | 0–5V |
| Resolution | 10-bit (0–1023) or 8-bit (0–255) |
| Linearity | Notable non-linearity — not for precision |

For precise measurements: use external ADC module.
For most tasks (0–100% regulators): built-in ADC is sufficient.

---

## 9. Analog Comparator

Comparator = 1-bit ADC. Compares two analog voltages:
- V+ > V− → output = 1 (high)
- V+ < V− → output = 0 (low)

Very high gain (10,000–1,000,000) — even minimal voltage difference causes switching.

**Practical use:** Power failure detector. Instantly detects voltage drop, generates interrupt, CPU saves state to EEPROM before power loss.

---

## 10. Watchdog Timer (WDT)

Protection against microcontroller freezing.

### How It Works
1. WDT = independent counter with own oscillator
2. Program must regularly reset WDT with `wdt_reset()` each loop
3. If program freezes → no resets → WDT overflows → processor reset
4. Microcontroller restarts

For critical systems (vending machines, industrial controllers): use external WDT module for maximum reliability.

---

## 11. Arduino Power

| Parameter | Value |
|-----------|-------|
| Max current per pin | 40 mA |
| Recommended current | 20 mA (50% of max) |
| VIN input | 5–8V recommended |

**Warning:** 12V on VIN causes internal regulator to overheat (12−5=7V dissipated as heat).
**Optimal:** USB (5V) or stabilized 5–7V source.

---

## 12. GPIO and Resistors

### Port Modes
| Mode | Use |
|------|-----|
| INPUT | Read external signal (button) |
| OUTPUT | Output signal (LED) |
| INPUT_PULLUP | Input with internal pull-up resistor to +5V |

**All used ports must be explicitly initialized in `setup()`.**

### Pull-up Resistor for Buttons
- Resistor (e.g., 10 kΩ) between +5V and input pin
- Button open → +5V on input (logic 1)
- Button pressed → input connected to GND (logic 0)
- Resistor prevents short circuit between +5V and GND

Arduino has built-in pull-ups: use `INPUT_PULLUP` mode.

---

## 13. Q&A

**Q: Do I need to reflash Arduino to work with ESP32 camera?**
A: No. Arduino keeps previous firmware. Disconnect Arduino during ESP32 programming, then reconnect.

**Q: Can I control the car with IR remote?**
A: Yes. Shield has IR receiver (VS1838B) on D3. Not included in standard kit. Works same way — IR commands instead of web commands.
