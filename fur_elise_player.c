// fur_elise_player.c
// Music Player for STM32L432KC with LM380 Audio Amplifier
//
// Author: Emmett Stralka
// Email: estralka@hmc.edu
// Date: 9/29/25
//
// Description: PWM-based music player with LM380 amplifier and hardware volume control

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_TIMER.h"

// ========== SONG SELECTION ==========
// Comment out one line and uncomment the other to switch songs
#define PLAY_FUR_ELISE
//#define PLAY_MINECRAFT
// ====================================

// External references to song data from lab4_starter.c
extern const int fur_elise_notes[][2];
extern const int minecraft_notes[][2];

// Function prototypes
void play_note(int frequency, int duration_ms);

// Define audio output pin
#define AUDIO_PIN 0  // PA0 for TIM2_CH1

// Main function - plays Für Elise with LM380 amplifier
int main(void) {
    configureFlash();
    configureClock();
    
    // Enable GPIOA clock
    RCC->AHB2ENR |= (1 << 0);
    
    // Configure PA0 as alternate function for TIM2_CH1
    volatile uint32_t *gpioa_moder = (volatile uint32_t *)0x48000000;
    *gpioa_moder &= ~(0b11 << 0);
    *gpioa_moder |= (0b10 << 0);
    
    // Set alternate function to AF1 (TIM2_CH1)
    volatile uint32_t *gpioa_afrl = (volatile uint32_t *)0x48000020;
    *gpioa_afrl &= ~(0b1111 << 0);
    *gpioa_afrl |= (0b0001 << 0);
    
    TIM2_Init();
    
    // Play selected song
    #ifdef PLAY_FUR_ELISE
        // Play Für Elise with proper timing
        // Fine-tune timing with 2x multiplier
        int i = 0;
        while (fur_elise_notes[i][1] != 0) {
            play_note(fur_elise_notes[i][0], fur_elise_notes[i][1] * 2);
            i++;
        }
    #endif
    
    #ifdef PLAY_MINECRAFT
        // Play Minecraft "Sweden" with proper timing
        // Fine-tune timing with 2x multiplier
        int i = 0;
        while (minecraft_notes[i][1] != 0) {
            play_note(minecraft_notes[i][0], minecraft_notes[i][1] * 2);
            i++;
        }
    #endif
    
    // End of song - infinite loop
    while(1) {
        // Song finished, stay here
    }
    return 0;
}

// Play a note with specified frequency and duration
void play_note(int frequency, int duration_ms) {
    if (frequency == 0) {
        // Rest - stop timer for silence
        TIM2_Stop();
        ms_delay(duration_ms);
        return;
    }
    
    // Set frequency using timer library
    TIM2_SetFrequency(frequency);
    
    // Play for specified duration
    ms_delay(duration_ms);
}