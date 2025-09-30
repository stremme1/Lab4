// fur_elise_player.c
// Music Player for STM32L432KC with Hardware Volume Control
//
// Author: Emmett Stralka
// Email: estralka@hmc.edu
// Date: 9/29/25
//
// Description: PWM-based music player with hardware volume control and 100 BPM timing

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"

// ========== SONG SELECTION ==========
// Comment out one line and uncomment the other to switch songs
#define PLAY_FUR_ELISE
//#define PLAY_MINECRAFT
// ====================================

// External references to song data from lab4_starter.c
extern const int fur_elise_notes[][2];
extern const int minecraft_notes[][2];

// Function prototypes
void TIM2_Init(void);
void play_note(int frequency, int duration_ms);
void ms_delay(int ms);

// Define audio output pin
#define AUDIO_PIN 0  // PA0 for TIM2_CH1

// Main function - plays Für Elise with hardware volume control
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
    
    // Test with A4 (440Hz)
    play_note(440, 2000);
    ms_delay(500);
    
    // Play selected song
    #ifdef PLAY_FUR_ELISE
        // Play Für Elise with original timing
        int i = 0;
        while (fur_elise_notes[i][1] != 0) {
            play_note(fur_elise_notes[i][0], fur_elise_notes[i][1]);
            i++;
        }
    #endif
    
    #ifdef PLAY_MINECRAFT
        // Play Minecraft "Sweden" with original timing
        int i = 0;
        while (minecraft_notes[i][1] != 0) {
            play_note(minecraft_notes[i][0], minecraft_notes[i][1]);
            i++;
        }
    #endif
    
    // End of song - infinite loop
    while(1) {
        // Song finished, stay here
    }
    return 0;
}

// Function for dummy delay by executing nops
void ms_delay(int ms) {
   while (ms-- > 0) {
      volatile int x=1000;
      while (x-- > 0)
         __asm("nop");
   }
}

// Timer definitions (tutorial style)
#define __IO volatile

// Base addresses
#define TIM2_BASE (0x40000000UL)

// Timer register structure (tutorial style)
typedef struct {
    __IO uint32_t CR1;      // Control register 1
    __IO uint32_t CR2;      // Control register 2
    __IO uint32_t SMCR;     // Slave mode control register
    __IO uint32_t DIER;     // DMA/interrupt enable register
    __IO uint32_t SR;       // Status register
    __IO uint32_t EGR;      // Event generation register
    __IO uint32_t CCMR1;    // Capture/compare mode register 1
    __IO uint32_t CCMR2;    // Capture/compare mode register 2
    __IO uint32_t CCER;     // Capture/compare enable register
    __IO uint32_t CNT;      // Counter
    __IO uint32_t PSC;      // Prescaler
    __IO uint32_t ARR;      // Auto-reload register
    uint32_t      RESERVED1; // Reserved
    __IO uint32_t CCR1;     // Capture/compare register 1
    __IO uint32_t CCR2;     // Capture/compare register 2
    __IO uint32_t CCR3;     // Capture/compare register 3
    __IO uint32_t CCR4;     // Capture/compare register 4
    uint32_t      RESERVED2; // Reserved
    __IO uint32_t DCR;      // DMA control register
    __IO uint32_t DMAR;     // DMA address for full transfer
} TIM_TypeDef;

#define TIM2 ((TIM_TypeDef *) TIM2_BASE)

// Initialize TIM2 for PWM generation
void TIM2_Init(void) {
    RCC->APB1ENR1 |= (1 << 0);
    
    // Simple setup: 1MHz timer, fixed 1kHz output
    TIM2->PSC = 79;  // 80MHz / 80 = 1MHz
    TIM2->ARR = 1000;  // 1kHz square wave
    TIM2->CCR1 = 500;  // 50% duty cycle
    
    // Enable PWM output
    TIM2->CCMR1 |= (0b110 << 4) | (1 << 3);
    TIM2->CCER |= (1 << 0);
    TIM2->CR1 |= (1 << 7) | (1 << 0);
}

// Play a note with specified frequency and duration
void play_note(int frequency, int duration_ms) {
    if (frequency == 0) {
        // Rest - turn off timer
        TIM2->CR1 &= ~(1 << 0);
    } else {
        // Note - turn on timer (fixed 1kHz square wave)
        TIM2->CR1 |= (1 << 0);
    }
    
    ms_delay(duration_ms);
}

