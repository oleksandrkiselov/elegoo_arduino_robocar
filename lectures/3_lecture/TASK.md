# Lecture 3 — Homework Assignment

## Tasks

1. **Install ESP32 support** in Arduino IDE
   - Install version **2.0.17** (do NOT use v3)
   - Add ESP32 board manager URL

2. **Download and configure ESP32-CAM code**
   - Set your WiFi SSID and password
   - Configure motor parameters to match your previous settings

3. **Program ESP32**
   - **Disconnect TX/RX lines** from Arduino before programming
   - Select board: AI Thinker ESP32-CAM
   - Upload firmware
   - Press Reset or power cycle if not auto-starting

4. **Test web interface**
   - Connect to camera stream
   - Test light on/off button
   - Verify video feed displays correctly
   - If image inverted — change `rotation` parameter to 180

5. **Verify command codes**
   - Ensure digital command codes match your Arduino motor settings from Lecture 2
   - Test all movement directions

---

## Important Notes

- Always disconnect TX/RX lines before programming ESP32
- Reconnect TX/RX after programming for car to work
- Use home WiFi router for best results (phone hotspot may have issues)
- If ESP32 doesn't start after flashing — press Reset or power cycle for 15 seconds
