# Lecture 2 — Homework Assignment

## Tasks

1. **Upload test program** to Arduino via Arduino IDE
   - Ensure board is set to "Arduino Uno"
   - Select correct COM port

2. **Check the switch position** on Elegoo Shield:
   - "upload" position during firmware upload
   - "cam" position before running

3. **Connect charged battery** — verify polarity (+/-)

4. **Run robot on flat floor** — confirm all motors spin

5. **Perform wheel calibration:**
   - Determine which bit corresponds to which wheel and direction
   - Test bits one at a time (1, 2, 4, 8, 16, 32, 64, 128)
   - Create mapping table

6. **Program basic movements:**
   - Forward
   - Backward
   - Left (strafe)
   - Right (strafe)
   - Rotate clockwise
   - Rotate counter-clockwise

7. **Record video** of the robot working and submit to the designated chat

---

## Troubleshooting

- **Motors not moving?**
  - Check screw terminal contacts (tighten with screwdriver)
  - Verify battery polarity
  - Ensure battery is charged

- **Robot not driving straight?**
  - Recalibrate wheel bits
  - Check all 4 motors are getting the "forward" signal
