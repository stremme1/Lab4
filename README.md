# Lab4 - Für Elise Player for STM32L432KC

This project implements a Für Elise player using the STM32L432KC microcontroller with PWM-based square wave generation and hardware volume control.

## BPM Calculation
- Simplified approach: Fixed 1kHz square wave output
- Original durations used: 125ms, 250ms, 375ms, 500ms
- No complex frequency calculations needed

## Specifications
- Min duration: 125ms (shortest note in Für Elise)
- Max duration: 500ms (longest note in Für Elise)  
- Min frequency: 262Hz (lowest note in Für Elise data)
- Max frequency: 1319Hz (highest note in Für Elise data)
- Timer output: Fixed 1kHz square wave (frequency parameter ignored)
- Duration range: 125ms to 500ms (4x range)
- Frequency range: 262Hz to 1319Hz (5x range)

The system uses 80MHz system clock with 1MHz timer frequency for precise audio generation. Hardware volume control via potentiometer eliminates need for ADC processing.