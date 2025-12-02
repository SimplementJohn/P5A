# P5A

ESP32 project that tracks real-time sun position (azimuth & altitude) using le-systeme-solaire.net API. WiFi-enabled with ArduinoJSON parsing for IoT solar tracking applications.

## 📁 Project Structure

### Main Files

| File | Description |
|------|-------------|
| `P5A-API_WIFI.ino` | WiFi connection and GET API requests with solar data processing (azimuth, altitude) |
| `P5A-MOTEUR.ino` | Stepper motor control test for dual-axis solar tracking system |
| `P5A-HOURREQUEST.ino` | Ask for HOUR time RN |

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

- ESP32 microcontroller
- A4988 stepper motor drivers
- Dual-axis stepper motors for solar tracking

## 🚀 Features

- Real-time sun position calculation via API
- WiFi connectivity for cloud data access
- JSON data parsing with ArduinoJSON
- Stepper motor control with A4988 drivers
- Dual-axis tracking capability

## 📡 API

This project uses [le-systeme-solaire.net](https://le-systeme-solaire.net) API to retrieve real-time solar position data (azimuth and altitude angles).
