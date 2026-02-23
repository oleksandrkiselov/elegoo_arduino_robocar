# Lecture 4 — Homework Assignment

## Task

**Run the servo motor on the robot and record a video.**

### Steps

1. **Write servo test program** for Arduino Uno (not ESP32)
   - Use `Servo.h` library (version 1.2.x)
   - Connect servo: Yellow → Pin 9, Brown → GND, Red → +5V

2. **Test servo positions**
   - 0° (full right)
   - 90° (straight ahead)
   - 180° (full left)

3. **Calibrate if needed**
   - Adjust `myServo.attach(9, 544, 2400)` parameters if servo doesn't hit positions accurately
   - Or mechanically adjust: unscrew head while powered, set to 90°, tighten

4. **Record video**
   - Keep phone/camera stationary
   - Place a book under the robot so it doesn't drive away during startup
   - Show servo moving through positions

5. **Submit video** for verification

---

## Example Code

```cpp
#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(9);
}

void loop() {
  myServo.write(0);   delay(5000);  // Full right
  myServo.write(90);  delay(5000);  // Straight
  myServo.write(180); delay(5000);  // Full left
}
```

---

## Safety Warnings

- ⚠️ **Never manually rotate servo while powered** — gears are fragile
- ⚠️ **Double-check wire connections** — wrong wiring can burn Pin 9
- Place book under robot to prevent it from driving off during testing
