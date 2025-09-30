# Lab4 - Für Elise Player for STM32L432KC

This project implements a Für Elise player using the STM32L432KC microcontroller with PWM-based square wave generation and hardware volume control.

## BPM Calculation
- Simplified approach: Fixed 1kHz square wave output
- Original durations used: 125ms, 250ms, 375ms, 500ms
- No complex frequency calculations needed

## Technical Constraints (Player Implementation)
- **Min duration**: 1ms (limited by ms_delay() precision at 80MHz)
- **Max duration**: 65,535ms (limited by 16-bit integer in ms_delay())
- **Min frequency**: 1Hz (theoretical minimum for timer)
- **Max frequency**: 1MHz (timer frequency limit: 80MHz ÷ 80 = 1MHz)
- **Actual output**: Fixed 1kHz square wave (frequency parameter ignored)
- **Timer resolution**: 1μs (1MHz timer frequency)
- **Duty cycle**: 50% (fixed CCR1 = ARR/2)

The system uses 80MHz system clock with 1MHz timer frequency for precise audio generation. Hardware volume control via potentiometer eliminates need for ADC processing.