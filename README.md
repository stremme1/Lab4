# Lab4 - Für Elise Player for STM32L432KC

This project implements a Für Elise player using the STM32L432KC microcontroller with PWM-based square wave generation.



## Features

- **80MHz System Clock**: Uses PLL configuration from tutorial solution
- **Square Wave Generation**: PWM-based audio output on PA5
- **Musical Notes**: Plays complete Für Elise melody
- **Accurate Timing**: 1MHz timer resolution for precise frequencies
- **Rest Support**: Handles silence between notes

## Hardware Setup

- **MCU**: STM32L432KC
- **Audio Output**: PA5 (TIM2_CH1)
- **Speaker**: Connect to PA5 and ground through simple amplifier

## Clock Configuration

- **System Clock**: 80MHz (PLL: M=1, N=80, R=4)
- **Timer Clock**: 1MHz (80MHz ÷ 80 prescaler)
- **Flash Waitstates**: 4 waitstates for 80MHz operation

## Audio Specifications

- **Frequency Range**: 262Hz - 1319Hz (covers all Für Elise notes)
- **Waveform**: 50% duty cycle square wave
- **Resolution**: 1μs timer resolution
- **Output**: PA5 (TIM2 Channel 1)

## Building and Flashing

1. **SEGGER Project**: Import `fur_elise_player.c` and `lab4_starter.c` into your SEGGER project
2. **Include Libraries**: Add the `lib/` folder to your include path
3. **Build**: Compile using SEGGER's build system
4. **Flash**: Use SEGGER's built-in J-Link integration to flash
5. **Listen**: Connect speaker to PA5 and ground

## Code Integration

The project uses the working tutorial solution for:
- Clock configuration (`configureClock()`)
- Flash setup (`configureFlash()`)
- GPIO functions (`pinMode()`)
- Delay functions (`ms_delay()`)

## Note Data

The `lab4_starter.c` file contains the complete Für Elise melody with:
- Frequencies in Hz
- Durations in milliseconds
- Rest periods (frequency = 0)

## Functions

- `TIM2_Init()`: Initialize timer for PWM generation
- `play_note(frequency, duration)`: Play a single note
- `ms_delay(ms)`: Precise delay function
- `configureClock()`: Setup 80MHz PLL
- `configureFlash()`: Configure flash for high-speed operation

## Ready for SEGGER!

This project is complete and ready to be imported into SEGGER Embedded Studio for building and flashing to your STM32L432KC development board.