# ESP32 Medibox 

**ESP32 Medibox** is an intelligent medicine box designed to help individuals manage their medication schedule efficiently. Powered by the **ESP32 microcontroller**, this project integrates multiple sensors and actuators for comprehensive medication management, ensuring that you never miss a dose again.

---

## Overview

This project utilizes various sensors, actuators, and communication modules controlled by the ESP32 to provide features like:

- **WiFi Connectivity** for remote monitoring via **MQTT**.
- **Temperature & Humidity Monitoring** with the **DHT22 sensor**.
- **Light Intensity Measurement** via **LDRs**, adjusting a servo motor as necessary.
- **Buzzer Notifications** for reminding users to take their medication on time.
- **Button Interface** for user interaction like canceling alarms or confirming actions.

Developed with the following tools:

[![VS Code](https://img.shields.io/badge/Editor-VS%20Code-blue.svg)](https://code.visualstudio.com/)
[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc/)
[![Node-RED](https://img.shields.io/badge/Tool-Node--RED-red.svg)](https://nodered.org/)
[![Wokwi](https://img.shields.io/badge/Simulation-Wokwi-green.svg)](https://wokwi.com/)

---


## Circuit Diagram

![Screenshot 2024-09-27 232921](https://github.com/user-attachments/assets/aa21ed51-3454-45a1-acfe-1733a0d69fda)


---

## DashBoard

![Screenshot 2024-09-27 234146](https://github.com/user-attachments/assets/1b74c568-cb0b-419a-8ddf-2d911265947c)

---

## Table of Contents

- [Features](#features)
- [Components](#components)
- [How It Works](#how-it-works)
- [MQTT Topics](#mqtt-topics)
- [Getting Started](#getting-started)
- [Simulating](#simulating)
- [License](#license)
- [Contact](#contact)

---

## Features

- **WiFi Connectivity**: Remote monitoring and control via **MQTT**.
- **Temperature & Humidity Monitoring**: Environmental tracking with **DHT22**.
- **Light Intensity Measurement**: **LDR** sensors to adjust **servo motor** based on light.
- **Servo Motor Control**: Automatically adjusts a shaded window based on ambient light.
- **Buzzer Notifications**: Audio alerts for scheduled medication times.
- **Button Interface**: Manual control for confirming actions or canceling alarms.
- **Customizable Settings**: Control **servo motor angles** and other parameters via **MQTT**.

---

## Components

- **ESP32**: Main microcontroller.
- **DHT22**: Sensor for temperature and humidity.
- **LDRs**: For detecting ambient light levels.
- **Servo Motor**: Moves a shaded window according to light levels.
- **Buzzer**: Provides audio notifications.
- **Buttons**: For manual user input and interaction.

---

## How It Works

The **ESP32 Medibox** helps users manage their medication schedules with real-time monitoring and notifications:

1. **Environmental Monitoring**: Constantly measures **temperature**, **humidity**, and **light intensity**.
2. **Reminders**: Uses a **buzzer** to notify users when it’s time to take medication.
3. **Remote Monitoring**: Users can monitor and control the device remotely using **MQTT**.
4. **Customizable Settings**: Adjust servo motor parameters (e.g., minimum angle) via MQTT.

---

## MQTT Topics

The following MQTT topics are used in this project:

| Topic            | Description                                         |
|------------------|-----------------------------------------------------|
| `MQTT-ON-OFF`    | Controls the buzzer (ON/OFF).                       |
| `MQTT-SCH-ON`    | Manages the schedule for buzzer notifications.      |
| `MQTT-TEMP`      | Receives temperature readings.                      |
| `MQTT-LDR`       | Receives light intensity data.                      |
| `MQTT-MIN-ANG`   | Sets the minimum angle of the servo motor.          |
| `MQTT-CTRL-FAC`  | Sets the controlling factor for the servo motor.    |
| `MQTT-DROP-DOWN` | Selects predefined settings for the servo motor.    |

---

## Getting Started

Follow these steps to get started with **ESP32 Medibox**:

1. **Clone the Repository**:
    ```bash
    git clone https://github.com/AnupaAlwis/smart-medi-box.git
    ```

2. **Install Dependencies**:
    - Install the following libraries via **Arduino IDE** Library Manager:
      - WiFi
      - PubSubClient
      - DHTesp
      - ESP32Servo
      - ArduinoJson

3. **Configure WiFi & MQTT**:
    - Update the `setupWifi()` and `setupMqtt()` functions in the code with your **WiFi credentials** and **MQTT broker details**.

---

## Simulating

You can simulate this project using **Wokwi** for VS Code:

1. Open the project in **VS Code**.
2. Press `F1` and select **Wokwi: Start Simulator**.
3. Interact with the simulation and test the functionality before deploying to real hardware.

---

## License

This project is licensed under the **MIT License**.

---

## Contact

For any questions or inquiries:

- GitHub: [AnupaAlwis](https://github.com/AnupaAlwis)
- Email: isurajithalwis@gmail.com

---


