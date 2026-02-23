/*
 * =============================================================================
 * Lecture 4 Homework: Servo Motor Test
 * ELEGOO Smart Robot Car V4.0
 * =============================================================================
 *
 * TASK: Run the servo motor on the robot and record a video showing it
 *       moving through 0°, 90°, and 180° positions.
 *
 * =============================================================================
 * SOURCES & REFERENCES:
 * =============================================================================
 *
 * [1] CLAUDE.md (project instructions file in repository root)
 *     - Pin assignments table states:
 *       "Servo Z (horizontal): Pin 10"
 *       "Servo Y (vertical): Pin 11"
 *
 * [2] Vendor PDF: docs/.../04 SmartRobotCarV4.0_ServoControl.pdf (page 3)
 *     - Shows servo headers labeled "P10" and "P11" on the shield diagram
 *     - Quote: "servo can be connected to the pin D10 or pin D11 of the UNO"
 *
 * [3] Vendor PDF: docs/.../04 SmartRobotCarV4.0_ServoControl.pdf (page 4)
 *     - Code shows: #define PIN_Servo_z 10
 *     - This confirms Pin 10 is for "Servo Z" (horizontal/pan axis)
 *
 * [4] Vendor PDF: docs/.../04 SmartRobotCarV4.0_ServoControl.pdf (page 5)
 *     - Pulse width calibration: myservo.attach(PIN_Servo_z, 500, 2400);
 *     - Comment: "500: 0 degree  2400: 180 degree"
 *
 * [5] Vendor demo code: docs/.../04 SmartRobotCarV4.0_ServoControl/Demo1/Demo1.ino
 *     - Uses DeviceDriverSet_Servo class which internally uses Pin 10
 *
 * [6] lectures/4_lecture/TASK.md (homework assignment)
 *     - Mentions Pin 9, but this appears to be a generic Arduino example
 *     - The actual ELEGOO V4.0 shield has servo headers on pins 10 and 11
 *
 * DECISION: Using Pin 10 because:
 *   - The robot car's shield physically connects servo headers to pins 10/11
 *   - All vendor code uses Pin 10 for the horizontal servo
 *   - Pin 9 on this robot is used for the IR receiver (per CLAUDE.md)
 *   - If you're using a loose servo not on the shield, you CAN use Pin 9
 *     by changing SERVO_PIN below
 *
 * =============================================================================
 * HARDWARE CONNECTIONS (on the ELEGOO Smart Robot Car shield):
 * =============================================================================
 *   - Servo Z (horizontal/pan): Pin 10  [Source: CLAUDE.md, vendor PDF page 3-4]
 *   - Servo Y (vertical/tilt):  Pin 11  [Source: CLAUDE.md, vendor PDF page 3]
 *
 *   Servo wire colors (standard for SG90 servos):
 *     - Yellow/Orange: Signal (connects to pin)
 *     - Brown/Black:   GND
 *     - Red:           +5V
 *
 * =============================================================================
 * INSTRUCTIONS - BEFORE UPLOADING:
 * =============================================================================
 * 1. Connect the robot car to your computer via USB cable
 *
 * 2. Make sure the "Upload-Cam" switch on the robot is set to "Upload" position
 *    (This routes USB to the Arduino UNO instead of the ESP32 camera module)
 *
 * 3. In Arduino IDE:
 *    - Select Board: "Arduino Uno"
 *    - Select the correct COM port (check Device Manager if unsure)
 *    - Make sure Servo library is installed (usually comes with Arduino IDE)
 *
 * 4. Upload the sketch
 *
 * =============================================================================
 * INSTRUCTIONS - RECORDING THE VIDEO:
 * =============================================================================
 * 1. Place a book or block under the robot so wheels don't touch the ground
 *    (prevents the robot from driving away during power-on initialization)
 *
 * 2. Position your camera/phone on a stable surface (tripod or propped up)
 *
 * 3. Turn on the robot car power switch
 *
 * 4. Record the servo cycling through:
 *    - 0°   (full right)  - 5 seconds
 *    - 90°  (center)      - 5 seconds
 *    - 180° (full left)   - 5 seconds
 *    Then repeats...
 *
 * =============================================================================
 * CALIBRATION (if servo doesn't hit positions accurately):
 * =============================================================================
 * Option A - Software calibration:
 *   Change the attach() call to include pulse width limits:
 *   myServo.attach(SERVO_PIN, 544, 2400);
 *
 *   Default values are 544us (0°) and 2400us (180°)
 *   Adjust these values if your servo doesn't reach full range:
 *   - Increase min value (e.g., 600) if 0° doesn't go far enough right
 *   - Decrease max value (e.g., 2300) if 180° doesn't go far enough left
 *
 * Option B - Mechanical calibration:
 *   1. Power on the servo and let it move to 90° position
 *   2. Carefully unscrew the servo horn (the part attached to the shaft)
 *   3. Reposition it so it points straight ahead at 90°
 *   4. Tighten the screw back
 *
 * =============================================================================
 * SAFETY WARNINGS:
 * =============================================================================
 *   - NEVER manually rotate the servo while it is powered!
 *     The internal gears are plastic and will strip easily.
 *
 *   - Double-check wire connections before powering on.
 *     Incorrect wiring can damage the servo or Arduino pin.
 *
 *   - The servo draws current when moving. If you see erratic behavior,
 *     make sure the battery is charged (7.2-8.4V range).
 *
 * =============================================================================
 */

#include <Servo.h>

// =============================================================================
// CONFIGURATION - Modify these values if needed
// =============================================================================

// Pin configuration for ELEGOO Smart Robot Car V4.0
// SOURCE: CLAUDE.md states "Servo Z (horizontal): Pin 10"
// SOURCE: Vendor PDF page 4 shows "#define PIN_Servo_z 10"
// NOTE: If using a loose servo (not on shield), you can change to Pin 9
//       as mentioned in TASK.md, but Pin 9 is the IR receiver on this robot.
#define SERVO_PIN 10

// Pulse width calibration (microseconds)
// SOURCE: Vendor PDF page 5: "myservo.attach(PIN_Servo_z, 500, 2400)"
//         with comment "500: 0 degree  2400: 180 degree"
// NOTE: Arduino Servo library defaults are 544 and 2400. The vendor uses 500.
#define SERVO_MIN_PULSE 500   // Pulse width for 0 degrees
#define SERVO_MAX_PULSE 2400  // Pulse width for 180 degrees

// Delay between position changes (milliseconds)
// SOURCE: TASK.md example code uses delay(5000) between positions
#define POSITION_DELAY 5000   // 5 seconds at each position

// =============================================================================
// GLOBAL VARIABLES
// =============================================================================

Servo myServo;  // Create servo object

// =============================================================================
// SETUP - Runs once when Arduino powers on or resets
// =============================================================================
void setup() {
  // Initialize serial communication for debugging (optional)
  // Open Serial Monitor at 9600 baud to see position messages
  Serial.begin(9600);
  Serial.println("=================================");
  Serial.println("Lecture 4 Homework: Servo Test");
  Serial.println("=================================");
  Serial.println();

  // Attach servo to pin with calibrated pulse widths
  // Parameters: pin, min pulse (0°), max pulse (180°)
  myServo.attach(SERVO_PIN, SERVO_MIN_PULSE, SERVO_MAX_PULSE);

  Serial.print("Servo attached to pin ");
  Serial.println(SERVO_PIN);
  Serial.println();

  // Move to center position initially
  Serial.println("Moving to initial position: 90 degrees (center)");
  myServo.write(90);
  delay(1000);  // Wait 1 second for servo to reach position

  Serial.println();
  Serial.println("Starting test cycle...");
  Serial.println("Each position will hold for 5 seconds.");
  Serial.println();
}

// =============================================================================
// LOOP - Runs repeatedly after setup()
// =============================================================================
void loop() {
  // ---------------------------------------------
  // Position 1: 0 degrees (full right)
  // ---------------------------------------------
  Serial.println(">>> Position: 0 degrees (FULL RIGHT)");
  myServo.write(0);
  delay(POSITION_DELAY);

  // ---------------------------------------------
  // Position 2: 90 degrees (center/straight ahead)
  // ---------------------------------------------
  Serial.println(">>> Position: 90 degrees (CENTER)");
  myServo.write(90);
  delay(POSITION_DELAY);

  // ---------------------------------------------
  // Position 3: 180 degrees (full left)
  // ---------------------------------------------
  Serial.println(">>> Position: 180 degrees (FULL LEFT)");
  myServo.write(180);
  delay(POSITION_DELAY);

  // ---------------------------------------------
  // Return to center before repeating
  // ---------------------------------------------
  Serial.println(">>> Position: 90 degrees (CENTER)");
  myServo.write(90);
  delay(POSITION_DELAY);

  Serial.println();
  Serial.println("--- Cycle complete, repeating... ---");
  Serial.println();
}

// =============================================================================
// END OF FILE
// =============================================================================
