# Mini Smart Home (ESP32-C3)

A mini smart-home prototype built using an ESP32-C3 with a cardboard house model.
Features include RFID door access, gas alert, motion alert, manual light switches, and a servo-controlled door.

## Features
- ✅ RFID access control (allowed UID opens door)
- ✅ Green LED = Access granted, Orange LED = Access denied
- ✅ PIR motion sensor → short beep on motion event
- ✅ Gas sensor → distinct alert pattern
- ✅ 2 lights controlled by physical switches
- ✅ Servo door mechanism (continuous servo timed open/close)

## Hardware Used
- ESP32-C3
- RC522 RFID reader
- PIR motion sensor
- Gas sensor module (DO pin)
- Active buzzer
- Continuous micro servo
- LEDs + resistors + breadboard + jumper wires
- Cardboard house model

## Wiring / Pins
| Module | Pin on module | ESP32-C3 Pin |
|------|---------------|-------------|
| RFID RC522 | SDA/SS | GPIO10 |
| RFID RC522 | RST | GPIO2 |
| Servo | Signal | GPIO3 |
| Buzzer | Signal | GPIO21 |
| LED (Green) | + | GPIO8 |
| LED (Orange) | + | GPIO9 |
| PIR | OUT | GPIO7 |
| Gas Sensor | DO | GPIO1 |
| All | GND | GND |
| RFID/Servo/Gas/PIR | VCC | 3.3V / 5V (as used) |

> Note: Make sure all grounds are common (GND connected together).

## Demo
- Video: (put your link here)
- Photos: see `/media/photos`

## How to Run
1. Open the `.ino` file in Arduino IDE
2. Select **ESP32-C3 board**
3. Install libraries:
   - `MFRC522`
   - `SPI` (built-in)
4. Upload to the ESP32-C3
5. Open Serial Monitor (115200)

## Future Improvements
- Use a real door latch mechanism
- Add OLED status screen
- Add Wi-Fi control / mobile dashboard
