# Lecture 4 Summary

**Topic:** Actuators and Robot Execution Mechanisms
**Instructor:** Viktor Rovinskyi

---

## 1. Stepper Motors

### Principle of Operation
Stepper motor = DC motor that moves in discrete steps. Contains magnet/toothed wheel and coils. Rotor seeks "tooth-to-tooth" alignment (stable state). Switching coils sequentially (1→2→3→4) rotates motor step by step.

Smaller tooth spacing = stronger magnetic field = stronger attraction. When coil activates, rotor "clicks" to nearest stable position.

### Operating Modes

| Mode | Description |
|------|-------------|
| Wave drive / Full-step | One coil at a time, 1 full step |
| Half-step | Two coils simultaneously, step halved |
| Micro-step (2/3 step) | Further step reduction |
| Analog mode | Smooth voltage for even smaller steps |

### Key Limitation
⚠️ **Motor doesn't know its position.** If shaft is blocked, it keeps sending pulses, unaware rotor is stuck. Always ensure motor power exceeds load.

### Applications
- CNC machines
- Robots and manipulators
- Previously: printers (now use other drive types)
- Arduino: cheap modules with stepper + gearbox available

### Stepper Motor Drivers

| Driver | Function |
|--------|----------|
| L297 | Controlled by Clock + Direction signals; generates coil switching sequence |
| L298 | Large driver on aluminum heatsink (cools to 125°C), contains Darlington transistors |

**Protection diodes:** Coils are inductive. Breaking circuit causes voltage spike (self-induction EMF). Diodes (conduct in one direction) protect against these spikes.

---

## 2. Servo Motors

### What is a Servo
Servo = motor + gearbox + angle sensor on output shaft. Implements feedback system: continuously compares current position with target, rotates until difference = 0.

### Comparison with Stepper Motor

| Feature | Stepper | Servo |
|---------|---------|-------|
| Position awareness | None | Continuous |
| External force response | No reaction | Returns to position |
| Complexity | Simpler | More complex, more reliable |
| Feedback | None | Encoder/potentiometer |

### Why Sensor is on Output
Each gearbox stage adds backlash (gap between teeth). When reversing, output shaft doesn't move until teeth engage. Measuring final output position, not motor position, ensures accuracy.

### PWM Control
Instead of analog voltage (noise-sensitive), servos use PWM — varying pulse width at constant amplitude. More noise-resistant: amplitude noise doesn't affect pulse width.

---

## 3. Servo on Arduino — Practical Setup

### Library and Connection
- **Library:** Servo.h (Michael Margolis)
- **Installation:** Arduino IDE → Library Manager → "Servo" → version 1.2.x
- **Wiring:**
  - Yellow (signal) → Pin 9
  - Brown → GND
  - Red → +5V

⚠️ **Never mix up wires — wrong connection can burn Pin 9!**

### Example Code

```cpp
#include <Servo.h>

Servo myServo;

void setup() { myServo.attach(9); }

void loop() {
  myServo.write(0);   delay(5000);
  myServo.write(90);  delay(5000);
  myServo.write(180); delay(5000);
}
```

### Calibration (544 and 2400 parameters)
`myServo.attach(9, 544, 2400)` — sets min/max pulse width in microseconds
- 544 = left limit (0°)
- 2400 = right limit (180°)
- If servo doesn't hit exactly 90° — adjust these values

**Mechanical adjustment:** Unscrew head while powered, set to 90°, tighten.

### Servo Positions

| Angle | Direction |
|-------|-----------|
| 0° | Full right |
| 90° | Straight ahead |
| 180° | Full left |

⚠️ **Never manually rotate servo while powered — gears are fragile and break easily!**

---

## 4. Industrial Servo Actuators

Complex industrial modules for robot manipulators contain:

| Component | Purpose |
|-----------|---------|
| Built-in motor + brakes | Brakes engaged by spring, released by current |
| Absolute encoder | Up to 25-bit precision (~360°/million divisions) |
| Temperature sensor | Overheat protection |
| Load sensor | Overload protection |
| Harmonic drive | Reduction ratio up to 300:1 |
| Interfaces | CAN bus, Modbus, RS-485 |

Different models by torque: smaller joints (neck) = less powerful; larger joints (knee) = more powerful.

---

## 5. Linear Actuators

Move forward/backward in straight line. Principle: motor rotates screw with fine thread, nut moves linearly.

### Characteristics
- **Self-locking:** High thread friction prevents manual movement
- **Efficiency:** ~60–70% (friction losses) — acceptable since resting after movement
- **Limit switches:** At end positions

### Position Measurement Methods
- Resistive strip (resistance measurement)
- Optical film with marks (like printers)
- Motor encoder pulse counting

---

## 6. Solenoids

Coil with spring-loaded rod (plunger). Only two states: retracted / extended. Impact operation — current pulse causes plunger to rapidly move by inertia.

### Applications
- Electronic locks
- Water valves (washing machines)
- Latches/fixers

For multiple positions: combine with rack gear and spring.

---

## 7. Nitinol Wire (Shape Memory Alloy)

Material with "shape memory." Cold = one length. Heated (by passing current) = contracts with great force, mimicking muscle.

### Specifications
- Search: "Nitinol wire" on eBay/AliExpress
- Example: 0.5mm diameter wire → 29N force at 2.7A for 1 second
- To return to original state: cool down (air flow or natural cooling)

Popular in Japan/West; used in micro-robotics.

---

## 8. Gearboxes and Mechanical Transmissions

### 8.1 Planetary Gear

Like solar system: central "sun" (rotor), "planets" (gears on carrier), "ring" (outer toothed ring). Compact, used in gear motors.

Three configurations depending on which element is fixed:
- Fixed ring → carrier rotates slower, same direction
- Fixed carrier → rotor and ring rotate opposite directions
- Fixed rotor → accelerator configuration

### 8.2 Harmonic Drive

Flexible toothed wheel (bronze/spring steel) deforms wave-like. Achieves high reduction ratio in one stage (up to 300:1). Widely used in robots. Hobbyists 3D-print them with sufficient accuracy.

### 8.3 Belt Drives

| Type | Features | Use |
|------|----------|-----|
| Flat belt | Axes must be parallel | Old factories, conveyors |
| V-belt | Self-tightens in groove, less slip | Car engines, machines |
| Round belt | Can twist, 90°+ angle transmission, dampens vibration | Tape recorders, telescopes |
| Timing belt | No slip, no backlash, like chain | 3D printers, robots |

**Important:** Belt acts as mechanical fuse. If mechanism jams, belt slips and protects motor. If belt too strong — motor can burn instead.

### 8.4 Capstan (Self-Tensioning Mechanism)

Thread/cable wraps several times around shaft with tension spring. Virtually no slip — precise linear movement control. Used in old radios and mercury mirror telescope (6m rubber drive to dampen motor vibration).

### 8.5 CVT (Continuously Variable Transmission)

Gearboxes with variable ratio:
- **V-belt CVT:** Pulleys compress/expand — most common
- **Rack-based:** More complex design
- **Cone CVT:** Belt moves along cones

---

## 9. Practical Q&A

| Question | Answer |
|----------|--------|
| Where is servo program uploaded? | To Arduino Uno (main board, not camera board) |
| Why rotate the head? | For ultrasonic sensor in maze: robot stops, rotates head, finds space, drives there |
| What's the bottom IR sensor for? | Line following. Can also detect stair edge (not guaranteed) |
| Can the shield be replaced? | No — specific (Elegoo SmartCar Shield). Motors, sensors, servo — replaceable separately |
| What not to do with powered servo? | Don't manually rotate — breaks gears |
| Course goal? | Understand problems and solutions. Full study takes ~1.5 years |
