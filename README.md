# Tea Machine Project

## Overview
This project is an automated tea brewing system using an Arduino. It ensures precise tea brewing by lowering and lifting the tea bag based on the amount of water detected in the cup. The system is designed to prevent over-steeping, which can cause a bitter taste.

The brewing time is automatically computed based on the cup's weight and can be adjusted manually by the user. An LCD display provides real-time feedback, and a servo motor controls the tea bag's movement.

## Features
- **Automated brewing time computation** based on liquid weight
- **Adjustable wait time** with increase and decrease buttons
- **LCD display** for real-time feedback
- **Servo-controlled tea bag handling**
- **Push-button controls** for easy operation
- **Load cell and HX711 amplifier** for weight measurement
- **Debounced button presses** for reliability

## Components Used
- **Microcontroller**: Arduino Uno
- **Display**: I2C LCD (16x2)
- **Motor**: MG90S Servo Motor
- **Weight Measurement**: 1kg Load Cell with HX711 Amplifier
- **User Input**: Push buttons (Start, Increase, Decrease)
- **Power Supply**: 5V

## Circuit Connections
| Component  | Pin Connection |
|------------|---------------|
| Servo Motor | Pin 4 |
| Start Button | Pin 5 |
| Load Cell DOUT | Pin 3 |
| Load Cell SCK | Pin 2 |
| Increase Button | Pin 6 |
| Decrease Button | Pin 7 |
| LCD | I2C (SDA, SCL) |

## How It Works
1. **Idle State**: Displays "Press Start" on LCD.
2. **Compute State**: Reads cup weight and calculates brewing time.
3. **Wait for Confirmation**: User can start brewing or adjust time.
4. **Brewing State**: Servo lowers the tea bag, countdown displayed on LCD.
5. **Complete State**: Tea bag is lifted, and "Enjoy your tea!" message is shown.

## Adjustments
- **Increase brewing time**: Press the Increase button (+10s per press, up to double the standard time).
- **Decrease brewing time**: Press the Decrease button (-10s per press, minimum 10s).

## Future Improvements
- Add a **temperature sensor** to ensure optimal brewing temperature.
- Improve **device aesthetics** for better integration into a home environment.
- Replace **Arduino Uno with Arduino Nano** for compact design.
- Solder components for a more stable setup.

![image](https://github.com/user-attachments/assets/0702557f-95b2-4e89-aa67-32003734f3f5)


