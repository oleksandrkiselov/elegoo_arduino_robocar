# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino project for the ELEGOO Smart Robot Car Kit V4.0, a four-wheeled robot car based on the ELEGOO UNO R3 (Arduino UNO compatible) controller.

## Hardware Specifications

| Component | Specification |
|-----------|---------------|
| Controller | ELEGOO UNO R3 |
| Input | Infrared Photoelectric Sensor, Ultrasonic Sensor, Buttons |
| Output | Motor, Servo Gimbal, LED |
| Programming Software | Arduino IDE, ElegooKit |
| Voltage | 7.2~8.4V (7.4V Lithium Battery Pack) |
| Battery Life | ~2 hours (Line-tracking Mode) |
| Distance Measuring | Ultrasonic Ranging Sensor |
| Motor Driver | Dual-channel TB6612 (or DRV8835 on older variants) |
| Tracking Method | Infrared Tracking (ITR20001) |
| Motor | DC Servo Motor 1:48 gear ratio |
| Communication | UART / WiFi |
| Camera | OV2640 |
| WiFi Module | ESP32-WROVER |
| Single-axis Gimbal | SG90 Servo |
| Size | 263 x 145 x 80 mm |
| Weight | 1140g |
| IMU | MPU6050 or QMI8658C (depending on board revision) |

## Build Commands

```bash
# Using Arduino CLI
arduino-cli compile --fqbn arduino:avr:uno 1_lesson_auto_move.ino
arduino-cli upload -p COM8 --fqbn arduino:avr:uno 1_lesson_auto_move.ino

# Verify serial port (Windows)
arduino-cli board list
```

## Pin Assignments

### TB6612 Motor Driver Variant
| Function | Pin |
|----------|-----|
| Motor A PWM | 5 |
| Motor B PWM | 6 |
| Motor A Direction | 7 |
| Motor B Direction | 8 |
| Standby | 3 |

### DRV8835 Motor Driver Variant
| Function | Pin |
|----------|-----|
| Motor A PWM | 5 |
| Motor B PWM | 6 |
| Motor A Direction | 8 |
| Motor B Direction | 7 |

### Sensors and Peripherals
| Function | Pin |
|----------|-----|
| Ultrasonic Trigger | 13 |
| Ultrasonic Echo | 12 |
| Servo Z (horizontal) | 10 |
| Servo Y (vertical) | 11 |
| IR Receiver | 9 |
| Line Track Left | A2 |
| Line Track Middle | A1 |
| Line Track Right | A0 |
| Battery Voltage | A3 |
| RGB LED (FastLED) | 4 |
| Key Button | 2 |

## Code Architecture

### Motor Control Pattern (DRV8835 with 74HCT595N shift register)
The `1_lesson_auto_move.ino` uses a shift register for motor direction control:
- Direction bits are shifted out via `shiftOut()` to the 74HCT595N
- PWM pins 5 and 6 control speed for motor groups
- Direction constants combine individual wheel bits using bitwise OR

### Vendor Code Structure (in `docs/`)
The vendor provides two architectural patterns:

1. **Simple demos** - Direct motor control in single `.ino` files
2. **Full application** - Object-oriented architecture with:
   - `DeviceDriverSet_xxx0.h/.cpp` - Hardware abstraction layer for all peripherals
   - `ApplicationFunctionSet_xxx0.h/.cpp` - High-level robot behaviors (tracking, obstacle avoidance, follow mode)
   - Main `.ino` calls `Application_FunctionSet` methods in a state machine loop

### Hardware Variants
Code exists for three board variants in `docs/.../02 Main Program`:
- `DRV8835/` - Older motor driver, uses MPU6050 IMU
- `TB6612 & MPU6050/` - Newer motor driver with MPU6050
- `TB6612 & QMI8658C/` - Newest variant with QMI8658C IMU

## Vendor Documentation

Tutorial PDFs in `docs/.../03 Tutorial & Code/` cover:
1. Development environment setup (Arduino IDE)
2. Motor movement control
3. Line tracking
4. Servo control
5. Obstacle avoidance
6. Follow mode
7. Additional features (IR remote, Bluetooth app)
8. DIY programming via mobile app

Datasheets for all chips are in `docs/.../04 Related chip information/`.
