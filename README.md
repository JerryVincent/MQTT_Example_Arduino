# ESP32 Soil Moisture Monitoring and MQTT Communication

This project demonstrates how to use an ESP32 microcontroller to monitor soil moisture levels and communicate the data via MQTT to a broker. The project also includes an RGB LED for visual feedback and supports secure Wi-Fi connections using WPA2 Enterprise (Eduroam).

## Table of Contents
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [MQTT Topics](#mqtt-topics)
- [License](#license)

## Features
- **Soil Moisture Monitoring**: Reads soil moisture levels using an analog sensor.
- **MQTT Communication**: Publishes soil moisture data to an MQTT broker.
- **Secure Wi-Fi Connection**: Connects to Eduroam or other WPA2 Enterprise networks.
- **RGB LED Feedback**: Provides visual feedback based on soil moisture levels and MQTT commands.
- **JSON Data Format**: Sends data in JSON format for easy parsing and integration.

## Hardware Requirements
- ESP32 microcontroller
- Soil moisture sensor
- RGB LED (WS2812)
- Jumper wires
- Breadboard (optional)

## Software Requirements
- Arduino IDE
- ESP32 board package for Arduino IDE
- Libraries:
  - `WiFi.h`
  - `PubSubClient.h`
  - `WiFiClientSecure.h`
  - `SPI.h`
  - `esp_wpa2.h`
  - `Freenove_WS2812_Lib_for_ESP32.h`
  - `ArduinoJson.h`

## Installation
1. **Install Arduino IDE**: Download and install the Arduino IDE from [arduino.cc](https://www.arduino.cc/en/software).
2. **Install ESP32 Board Package**:
   - Open Arduino IDE, go to `File > Preferences`.
   - Add the following URL to the "Additional Boards Manager URLs" field:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools > Board > Boards Manager`, search for `esp32`, and install the package.
3. **Install Required Libraries**:
   - Go to `Sketch > Include Library > Manage Libraries`.
   - Search for and install the following libraries:
     - `PubSubClient`
     - `WiFiClientSecure`
     - `Freenove_WS2812_Lib_for_ESP32`
     - `ArduinoJson`

## Configuration
1. **Wi-Fi Settings**:
   - Modify the `EAP_IDENTITY`, `EAP_USERNAME`, and `EAP_PASSWORD` in the code to match your Eduroam credentials.
   - Set the `ssid` to `eduroam` or your network's SSID.

2. **MQTT Broker Settings**:
   - Replace `mqtt_server`, `mqtt_username`, and `mqtt_password` with your MQTT broker details.
   - Set the `mqtt_port` to `8883` for secure connections.

3. **Soil Moisture Sensor Pin**:
   - Ensure the soil moisture sensor is connected to the correct pin (default is `3`).

4. **RGB LED Configuration**:
   - The RGB LED is connected to pin `1` by default. Adjust the pin number in the `Freenove_ESP32_WS2812` initialization if necessary.

## Usage
1. **Upload the Code**:
   - Connect your ESP32 to your computer via USB.
   - Select the correct board and port in the Arduino IDE (`Tools > Board` and `Tools > Port`).
   - Upload the code to the ESP32.

2. **Monitor Serial Output**:
   - Open the Serial Monitor (`Tools > Serial Monitor`) to view the ESP32's output, including Wi-Fi connection status and MQTT messages.

3. **MQTT Communication**:
   - The ESP32 will publish soil moisture data to the `garden/soil_moisture/status/Jerry` topic.
   - The RGB LED will change color based on the soil moisture level and MQTT commands.

## MQTT Topics
- **Publish**:
  - `garden/soil_moisture/status/Jerry`: Soil moisture data in JSON format.
  
- **Subscribe**:
  - `set/led/rgb`: Controls the RGB LED (e.g., send `1` to turn the LED green).
  - `garden/soil_moisture/status/ishan`: Example topic for receiving commands or data.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

Feel free to contribute to this project by submitting issues or pull requests. Happy coding! 🌱
