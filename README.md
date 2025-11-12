# Maize Storage Management System

An Arduino-based monitoring and control system for maize storage facilities that tracks environmental conditions and detects pests.

## 🌟 Features

- **Environmental Monitoring**
  - Real-time temperature and humidity tracking using DHT11 sensor
  - Gas level detection with MQ-2 sensor for air quality monitoring
  
- **Pest Detection & Control**
  - Motion detection using PIR sensor to identify pest activity
  - Automated fumigation control via relay
  - Visual alerts with LED indicators

- **User Interface**
  - 16x2 LCD display with I2C interface
  - Automatic display cycling between different sensor readings
  - Emergency alerts for motion detection

## 🛠 Hardware Components

- **Microcontroller**: ESP32
- **Sensors**:
  - DHT11 (Temperature & Humidity)
  - MQ-2 Gas Sensor
  - PIR Motion Sensor
- **Display**: 16x2 LCD with I2C (Address: 0x27)
- **Actuators**:
  - Relay Module (Active Low)
  - LED Indicator
- **Power**: 5V DC

## 📋 Pin Configuration

| Component | GPIO Pin | Notes |
|-----------|----------|-------|
| DHT11 | GPIO 4 | Temperature & Humidity |
| MQ-2 Analog | GPIO 34 | Gas level reading |
| MQ-2 Digital | GPIO 35 | Optional digital reading |
| PIR Sensor | GPIO 32 | Motion detection |
| Relay | GPIO 13 | Fumigation control (Active Low) |
| LED | GPIO 25 | Motion indicator |

## 🔧 Setup Instructions

### 1. Library Dependencies
Install the following Arduino libraries:
- `DHT sensor library` by Adafruit
- `LiquidCrystal I2C` by Frank de Brabander

### 2. Hardware Connections
1. Connect all sensors to their respective GPIO pins as specified above
2. Ensure proper power supply (5V) for all components
3. Connect LCD I2C module to SDA/SCL pins (typically GPIO 21/22 on ESP32)

### 3. Configuration Notes
- **Relay Logic**: The system uses ACTIVE LOW relay configuration
- **PIR Sensor**: Configured with internal pull-up resistor
- **Display**: I2C address set to 0x27 (adjust if different)

## 📊 System Operation

### Display Modes
The system cycles through three display modes every 5 seconds:

1. **Temperature & Humidity**
   - Shows current temperature in °C and humidity in %

2. **Gas Levels**
   - Displays MQ-2 sensor readings with safety indicators:
     - SAFE: < 1000
     - MED: 1000-2000  
     - HIGH: > 2000

3. **Motion Status**
   - Shows pest detection status
   - "NO PEST" when no motion detected
   - "DETECTED" when motion detected

### Alert System
- **Motion Detection**: Triggers immediate fumigation (relay OFF) and LED indicator
- **Visual Alert**: Blinking LCD backlight during motion events
- **Automatic Recovery**: System returns to normal after 10 seconds of no motion

## 🚀 Usage

1. Upload the code to your ESP32
2. The system will initialize with a startup sequence
3. Normal operation begins with sensor monitoring
4. The LCD will automatically cycle through sensor readings
5. Motion detection will trigger immediate fumigation response

## 🔍 Serial Monitoring

The system outputs detailed sensor readings to Serial Monitor at 115200 baud:
- Temperature and humidity values
- Gas sensor readings
- Motion detection status
- Relay and LED states

## ⚠️ Important Notes

- Ensure proper calibration of MQ-2 sensor for your environment
- Adjust motion detection sensitivity based on installation location
- Verify relay logic matches your specific relay module
- The system uses internal pull-up resistors for PIR sensor

## 🐛 Troubleshooting

- **LCD not displaying**: Check I2C address and connections
- **Incorrect temperature readings**: Verify DHT11 wiring and library
- **False motion detection**: Adjust PIR sensor sensitivity and positioning
- **Relay not triggering**: Confirm ACTIVE LOW configuration matches your hardware

## 📝 License

This project is open source and available under the [MIT License](LICENSE).

---

**Developed by**: ZetaEngineering INC  
**Last Updated**: December 2024
