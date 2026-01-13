# Portable Security Field Generator

An Arduino-based indoor motion detection system featuring automatic calibration and stealth mode. Designed to be integratable into the Google Home ecosystem via Smart Plug.

## Description
This project creates an invisible ultrasonic tripwire. It is designed to be tucked under furniture or in narrow gaps to monitor foot traffic. Unlike traditional security sensors, this device adapts to its surroundings every time it is powered on, allowing for zero-config portability across different rooms.

## Key Features
- **Auto-Calibration:** Measures and sets its own trigger threshold on boot.
- **Stealth-First Design:** Starts in total darkness; LCD and LEDs remain off to prevent detection. On-board button (D3) allows to toggle off stealth mode, resulting in flashing LEDs and LCD printout accompanying piezo siren (upon trigger).
- **Smart Home Ready:** Integrated with Google Home via a Smart Plug for remote arming/disarming and even automated scheduling (e.g., "Activate security field at 10 PM").
- **High-Impact Alarm:** 3000Hz–4500Hz siren sweep with dual-LED strobe.
- **Debugging Data:** Manual button toggle (D3) to wake the display and view real-time sensor data.

## Hardware Mapping

| Component | Pin |
| :--- | :--- |
| Ultrasonic Trig | D9 |
| Ultrasonic Echo | D4 |
| Piezo Buzzer | D11 |
| LEDs (Strobe) | D12, D13 |
| Toggle Button | D3 |
| LCD (I2C) | A4(SDA), A5(SCL) |



## Google Home Setup
1. Connect the Arduino power supply to a Google Home-compatible **Smart Plug**.
3. Use voice commands to arm the device.
4. Set a **Routine** to power the device during specific hours for automated scheduling.

## Installation & Use
1. Place the device in the desired location.
2. Power on via the Smart Plug.
3. Stay clear of the sensor for 7 seconds during the silent calibration phase.
4. A short beep will confirm the system is ARMED - the device has now begun monitoring.
5. The HC-SR04's "security field" will be determined after calibration, and movement detected in the range will trigger alarm.
