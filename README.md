# Lab4 - Für Elise Player for STM32L432KC

This project implements a Für Elise player using the STM32L432KC microcontroller with PWM-based square wave generation and hardware volume control.

## BPM Calculation
- **125ms = quarter note** → **500ms at 100 BPM**
- **80MHz clock compensation**: 80ms ÷ 500ms × 10 = **16x multiplier**

## Specifications
- **Min duration**: 125ms (base unit)
- **Max duration**: 500ms (4x base)  
- **Min frequency**: 262Hz
- **Max frequency**: 1319Hz
- **Timer resolution**: 1μs (1MHz timer frequency)

The system uses 80MHz system clock with 1MHz timer frequency for precise audio generation. Hardware volume control via potentiometer eliminates need for ADC processing.