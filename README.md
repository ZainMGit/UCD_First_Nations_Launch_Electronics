# First Nations Launch 2025 – UC Davis Payload System

## Overview

This repository documents the payload system used by the University of California, Davis in the **2025 First Nations Launch Rocketry Competition** for the Moon challenge. The payload collects in-flight motion, rotation and pressure data using sensors sensors, and logs this data to an SD card throughout the rocket’s flight.

### 🥇 Awards Received In Competition
- 1st Place Overall  
- 1st Place in Written Report  
- 1st Place in Oral Presentation
![image](https://github.com/user-attachments/assets/511e701e-2962-491f-b917-1b58506b4086)

---

## 🔧 Components

- **Arduino Uno R4**
- **MicroSD Card Reader and MicroSD Card** 
- **9V Battery**
- **BMP280** – Pressure & Temperature Sensor  
- **SparkFun 9DoF IMU Breakout** (ISM330DHCX + MMC5983MA) – IMU Sensor

### Wiring Notes

- Both sensors communicate over I²C. Their addresses **do not conflict**, allowing them to share the same I²C bus.
- If using **Qwiic connectors**:
  - **Blue wire** → SDA  
  - **Yellow wire** → SCL
- On the **Arduino Uno**, the I²C pins are:
  - **A4** → SDA  
  - **A5** → SCL
- It is **recommended to use screw terminal blocks** to secure wires for flight reliability.

---

## 📊 Data Logging

Sensor data is logged every **2 seconds** into separate `.txt` files on the SD card:

### BMP280 (Environmental Data)
- Temperature (°C)
- Pressure (Pa)
- Calculated Altitude (m)

### ISM330DHCX (IMU Data)
- Acceleration (mg)
- Angular Velocity (mdps)

---

## 📁 Output Files

- `pressure_data.txt` – Logs temperature, pressure, and altitude  
- `motion_data.txt` – Logs acceleration and angular velocity

Each log entry is timestamped for post-flight analysis and synchronization.

---

## 📬 Contact

For any inquiries, please reach out to the UC Davis First Nations Launch team.

