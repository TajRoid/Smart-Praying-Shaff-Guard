# 🕋 Smart Praying Shaff Guard

An Arduino-based smart system designed to assist worshippers during prayer (Salah) by automatically counting prayer units (Raka'at) using a 7-segment display and protecting the prayer space (Sutrah) by blocking obstacles/passersby with a servo gate and alarm.

![Status](https://img.shields.io/badge/status-completed-brightgreen)
![Date](https://img.shields.io/badge/date-December%202025-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Category](https://img.shields.io/badge/category-IoT%20%2F%20Embedded%20Systems-orange)

---

## 📌 Features

- **Automated Raka'at Counter:** Detects prostration (Sujud) motion via Ultrasonic Sensor A. Every two prostrations increment the Raka'at count, displayed live on a 7-segment display.
- **Sutrah / Barrier Protection:** Uses Ultrasonic Sensor B to monitor the space directly in front of the worshipper.
- **Active Obstacle Defense:** Triggers an audible buzzer alarm and deploys a servo barrier gate when a passerby crosses into the prayer area.

---

## 🛠️ Hardware Components

| Component | Pin Assignment | Description |
|---|---|---|
| **Arduino Uno / Nano** | Microcontroller | Central processing unit |
| **Ultrasonic Sensor HC-SR04 (A)** | `Trig: 10`, `Echo: 9` | Detects head distance during Sujud |
| **Ultrasonic Sensor HC-SR04 (B)** | `Trig: 12`, `Echo: 11` | Detects passersby entering the prayer space |
| **Servo Motor (SG90)** | `Pin A5` | Operates the safety barrier gate |
| **Piezo Buzzer** | `Pin A4` | Auditory warning signal for passersby |
| **7-Segment Display (Common Anode)** | `A-G: Pins 2 to 8` | Displays current Raka'at count ($0 - 9$) |

---

## 🔌 Wiring Diagram & Configuration

### 7-Segment Display (Common Anode)
- **Pins 2–8** connect to segments **A through G** respectively.
- Common Anode pin connected to **5V**.

### Ultrasonic Sensors
- **Sensor A (Prostration):** `Trig -> 10`, `Echo -> 9`
- **Sensor B (Passerby Guard):** `Trig -> 12`, `Echo -> 11`

### Actuators
- **Servo Motor Signal:** `Pin A5`
- **Buzzer Positive Pin:** `Pin A4`

---

## ⚙️ How It Works

```mermaid
flowchart TD
    Start([START]) --> Loop[Main Loop Scan]
    
    Loop --> SensorA[Ultrasonic A: Sujud Sensor]
    SensorA --> CheckA{Distance < 15 cm?}
    CheckA -- YES --> IncSujud[Increment Sujud Count]
    IncSujud --> CheckSujud{Sujud Count == 2?}
    CheckSujud -- YES --> IncRakaat[Rakaat + 1<br/>Reset Sujud Count<br/>Update 7-Seg Display]
    CheckSujud -- NO --> SensorB
    IncRakaat --> SensorB
    CheckA -- NO --> SensorB[Ultrasonic B: Guard Sensor]

    SensorB --> CheckB{Distance < 60 cm?}
    CheckB -- YES --> TriggerAlarm[Sound Buzzer<br/>Rotate Servo Gate to 90°<br/>Hold 2 Seconds<br/>Reset Gate & Silence Buzzer]
    TriggerAlarm --> Loop
    CheckB -- NO --> Loop
