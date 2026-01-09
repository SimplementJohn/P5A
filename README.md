# P5A

ESP32 project for solar tracking system with optical sensor integration. Features stepper motor control, WiFi connectivity, and real-time sun position tracking using le-systeme-solaire.net API.

## 📁 Project Structure

### Main Files

| File | Description |
|------|-------------|
| `P5A-API_WIFI.ino` | WiFi connection and GET API requests with solar data processing (azimuth, altitude) |
| `P5A-MOTEUR.ino` | Stepper motor control test for dual-axis solar tracking system (400 steps/revolution with 1:2 gear ratio) |
| `P5A-MOTEUR-OPB815WZ.ino` | **Advanced motor control with OPB815WZ optical sensor integration** |
| `P5A-OPB815WZ.ino` | Optical sensor testing and calibration |
| `P5A-HOURREQUEST.ino` | Time synchronization and hour requests |

### Sensor Integration

| File | Description |
|------|-------------|
| `P5A-MOTEUR-OPB815WZ.ino` | Motor control with automatic direction switching on beam detection |
| `P5A-OPB815WZ.ino` | Standalone optical sensor testing with LED feedback |

### Support Files

The following files are provided by M. ALONZO for motor control integration:

| File | Description |
|------|-------------|
| `MotorControl.h` | Motor control library header |
| `ServoUtils.h` | Servo utilities for motor positioning |
| `RobotReceiverBaseFullServoMoteurs.ino` | Full servo motor receiver implementation |
| `Mecatronique-PCB_Controller_v3.pdf` | PCB controller documentation |
| `README.md` | This file |

## 🔧 Hardware

- **ESP32 microcontroller**
- **A4988 stepper motor drivers**
- **Dual-axis stepper motors** (400 steps/revolution with 1:2 gear ratio)
- **OPB815WZ optical sensor** for beam detection
- **LED indicators** for visual feedback

### OPB815WZ Sensor Wiring
```
OPB815WZ (4 fils) -> ESP32
- Rouge  (Anode)     -> 3V3 via résistance 220Ω
- Noir   (Cathode)   -> GND
- Blanc  (Collector) -> GPIO27 (pull-up interne)
- Vert   (Emitter)   -> GND
```

## 🚀 Features

### Core Functionality
- **Real-time sun position calculation** via API
- **WiFi connectivity** for cloud data access
- **JSON data parsing** with ArduinoJSON
- **Stepper motor control** with A4988 drivers
- **Dual-axis tracking capability**

### Advanced Features
- **Optical beam detection** with OPB815WZ sensor
- **Automatic direction switching** on beam interruption
- **LED feedback** for sensor state indication
- **Continuous rotation** until detection
- **Configurable step delays** (2000μs default)
- **Gear ratio support** (1:2 for mechanical advantage)

### P5A-MOTEUR-OPB815WZ.ino Behavior
1. **Motor rotates continuously** in current direction
2. **When beam is detected** (sensor = LOW):
   - Motor stops
   - LED turns ON
   - 1-second pause
   - Direction reverses
   - Motor resumes in opposite direction
3. **Cycle repeats** indefinitely

## 📡 API

This project uses [le-systeme-solaire.net](https://le-systeme-solaire.net) API to retrieve real-time solar position data (azimuth and altitude angles).

## ⚙️ Configuration

### Motor Settings
- **Steps per revolution**: 400 (200 base × 2 for 1:2 gear ratio)
- **Step delay**: 2000μs (adjustable via `STEP_DELAY_US`)
- **Motor pins**: EN=4, DIR_A=16, STEP_A=17, DIR_B=15, STEP_B=2

### Sensor Settings
- **Sensor pin**: GPIO27
- **LED pin**: GPIO2
- **Logic**: HIGH = beam clear, LOW = beam interrupted
