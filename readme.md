# Kitchen Timer

This is a simple kitchen timer that can be used to time your cooking. It is a simple timer that can be set to a specific time and will alert you when the time is up.

## Hardware

- WeAct Black Pill - STM32F401CCU6
- Rotary Encoder with Push Button
- SSD1306 OLED Display (128x32)
- Buzzer

## Usage

At any point in time, rotating the encoder will cause the timer to enter the time select state. When selecting the time, rotating the encoder will incriment/decriment unit by 1. Pressing the encoder will move to the next unit. When left alone for 5 seconds, the timer will begin.

When the timer is running, the display will show the time remaining. The buzzer will sound when the timer reaches 0. Pressing the encoder will pause the timer. Pressing the encoder again will start the timer from where it left off.

When the timer is beeping, pressing the encoder will stop the beeping and reset the timer.
