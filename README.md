# 🚀 ESP32 GSM Smart Security System

Advanced ESP32 based Smart Security System with:

- GSM Calling
- SMS Alert
- PIR Motion Detection
- Flame Detection
- Vibration Detection
- Door Detection
- RTC Time Control
- Smart Auto Fan
- Night Security Automation
- Cooldown Protection System

---

# 📌 Features

## ✅ Smart Night Security Mode

System works ONLY between:

```txt
9:00 PM → 5:00 AM
```

Day time:
```txt
Security OFF
```

Night time:
```txt
Security ACTIVE
```

---

# ✅ PIR Motion Detection

Detects:
- Human movement
- Motion inside room
- Intruder activity

Includes:
- Smart filtering
- False trigger reduction
- Motion timer

---

# ✅ Flame Detection

Detects:
- Fire
- Lighter flame
- Heat source

---

# ✅ Vibration Detection

Detects:
- Metal vibration
- Door hit
- Pump machine vibration
- Forced entry impact

---

# ✅ Door Monitoring

Detects:
- Door open
- Unauthorized access

---

# ✅ GSM SMS Alert

When any threat detected:
- SMS sent to all users instantly

---

# ✅ GSM Calling System

After SMS:

System calls all users one by one.

Features:
- 30 sec call timeout
- Press 1 to confirm alert
- Rejected call also counts as response
- Waiting list retry system
- Unlimited retry loop until all users respond

---

# ✅ Smart Fan Automation

Normal Mode:
```txt
Every 1 hour → Fan ON for 10 minutes
```

Emergency Mode:
```txt
Any sensor active → Fan instantly ON
```

---

# ✅ Cooldown System

After successful alert completion:

```txt
15 minute full cooldown
```

During cooldown:
- All sensors OFF
- GSM OFF
- Monitoring paused

---

# 🧠 Technologies Used

- ESP32
- SIM800L / GSM Module
- DS3231 RTC
- PIR Sensor
- Flame Sensor
- Vibration Sensor
- Reed Switch
- Relay Module

---

# 📦 Required Components

| Component | Quantity |
|---|---|
| ESP32 Dev Board | 1 |
| SIM800L GSM Module | 1 |
| DS3231 RTC Module | 1 |
| PIR Sensor | 1 |
| Flame Sensor | 1 |
| SW420 Vibration Sensor | 1 |
| Reed Switch | 1 |
| Relay Module | 1 |
| Cooling Fan | 1 |
| Jumper Wires | Required |
| Breadboard | Optional |

---

# 🔌 ESP32 Connection Diagram

---

## 📡 GSM Module

| GSM Module | ESP32 |
|---|---|
| TX | GPIO16 |
| RX | GPIO17 |
| GND | GND |
| VCC | External Power |

---

## 🕒 RTC Module

| RTC | ESP32 |
|---|---|
| SDA | GPIO21 |
| SCL | GPIO22 |
| VCC | 3.3V |
| GND | GND |

---

## 👤 PIR Sensor

| PIR | ESP32 |
|---|---|
| OUT | GPIO13 |
| VCC | 5V |
| GND | GND |

---

## 🔥 Flame Sensor

| Flame Sensor | ESP32 |
|---|---|
| DO | GPIO26 |
| VCC | 3.3V |
| GND | GND |

---

## 🔨 Vibration Sensor

| Vibration Sensor | ESP32 |
|---|---|
| DO | GPIO27 |
| VCC | 5V |
| GND | GND |

---

## 🚪 Reed Switch

| Reed Switch | ESP32 |
|---|---|
| OUT | GPIO14 |
| GND | GND |

---

## 💨 Relay Module

| Relay Module | ESP32 |
|---|---|
| IN | GPIO32 |
| VCC | 5V |
| GND | GND |

---

# 💻 Laptop Setup Guide

---

# ✅ Install Arduino IDE

Download:
```txt
https://www.arduino.cc/en/software
```

Install normally.

---

# ✅ Install ESP32 Board

Open Arduino IDE:

```txt
File → Preferences
```

Paste:

```txt
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Then:

```txt
Tools → Board → Board Manager
```

Search:
```txt
ESP32
```

Install:
```txt
esp32 by Espressif Systems
```

---

# ✅ Install Drivers

---

## CP210x Driver

Download:
```txt
https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers
```

---

## CH340 Driver

Download:
```txt
https://sparks.gogo.co.nz/ch340.html
```

---

# ✅ Select Board

```txt
Tools → Board → ESP32 Dev Module
```

---

# ✅ Select COM Port

```txt
Tools → Port → COMx
```

---

# ⚠️ Upload Error Fix

If upload fails:

Hold:
```txt
BOOT button
```

while uploading.

Release after:
```txt
Connecting.....
```

appears.

---

# ⚠️ Serial Monitor Garbage Text Fix

Set:

```txt
115200 baud
```

inside Serial Monitor.

---

# 🧰 Required Libraries

Install from:
```txt
Library Manager
```

---

## Libraries

```txt
RTClib
Wire
```

---

# 🔥 Flame Sensor Logic

| Value | Meaning |
|---|---|
| 0 | Fire Detected |
| 1 | Safe |

---

# 🔨 Vibration Sensor Logic

| Value | Meaning |
|---|---|
| 0 | Vibration Detected |
| 1 | Normal |

---

# 🚪 Door Sensor Logic

| Value | Meaning |
|---|---|
| 1 | Door Open |
| 0 | Door Closed |

---

# 👤 PIR Logic

| Value | Meaning |
|---|---|
| 1 | Motion |
| 0 | No Motion |

---

# 📱 GSM Features

## SMS Alert

All users receive:
```txt
SECURITY ALERT
Pump Room Threat Detected
```

---

## Calling Features

- Auto calling
- Retry system
- DTMF support
- Waiting list system
- 30 sec timeout
- Call confirmation

---

# 🧠 Smart Logic

## Sequence

```txt
Threat Detected
↓
SMS Sent
↓
Calling Started
↓
Waiting List Retry
↓
All Users Respond
↓
15 Minute Cooldown
↓
System Restart
```

---

# 🔥 PIR Calibration

After startup:
```txt
Wait 30 seconds
```

Do not move during calibration.

---

# 📟 Serial Monitor Dashboard

Shows:

- Date
- Time
- Sensor Status
- RAW Values
- GSM Activity
- Call Status
- SMS Status
- Fan Status
- Security Mode
- Smart Analysis

---

# 📂 Project Structure

```txt
ESP32-GSM-Smart-Security-System/
│
├── main.ino
├── README.md
├── circuit-diagram.png
├── screenshots/
└── docs/
```

---

# 🚀 Future Improvements

- WiFi Dashboard
- Mobile App
- Telegram Alert
- Cloud Logging
- Camera Integration
- AI Threat Detection
- Face Recognition

---

# 👨‍💻 Developer

Developed by:
```txt
SUBHADEEP JANA
```

Project:
```txt
ESP32 GSM Smart Security System
```

---

# ⭐ GitHub

If you like this project:
- Star the repository
- Fork the project
- Share with others

---

# ⚠️ Disclaimer

This project is for:
- Educational
- Research
- Automation
- Security Learning

purposes only.

Use responsibly.

