// fur_elise_player_hardware_volume.c
// Complete Für Elise Player for STM32L432KC with Hardware Volume Control
// Based on tutorial clock configuration solution
// Potentiometer controls volume directly in hardware (no ADC needed)

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"

// External reference to the note data from lab4_starter.c
extern const int notes[][2];

// Function prototypes
void TIM2_Init(void);
void play_note(int frequency, int duration_ms);
void ms_delay(int ms);

// Define audio output pin
#define AUDIO_PIN 5  // PA5 for TIM2_CH1

// Main function - plays Für Elise with hardware volume control
int main(void) {
    // Configure flash to add waitstates to avoid timing errors
    configureFlash();

    // Setup the PLL and switch clock source to the PLL
    configureClock();

    // Turn on clock to GPIOA
    RCC->AHB2ENR |= (1 << 0);

    // Set AUDIO_PIN as alternate function
    pinMode(AUDIO_PIN, GPIO_ALT);
    
    // Initialize timer for PWM generation
    TIM2_Init();
    
    // Play Für Elise
    // Volume is controlled by potentiometer in hardware
    int i = 0;
    while (notes[i][1] != 0) { // Loop until duration is 0 (end marker)
        if (notes[i][0] == 0) {
            // Rest - just delay
            ms_delay(notes[i][1]);
        } else {
            // Play note (volume controlled by hardware potentiometer)
            play_note(notes[i][0], notes[i][1]);
        }
        i++;
    }
    
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
    // Enable TIM2 clock
    RCC->APB1ENR1 |= (1 << 0);
    
    // Configure TIM2 for PWM mode
    // Prescaler: 80MHz / 80 = 1MHz timer frequency
    TIM2->PSC = 79; // 80MHz / (79+1) = 1MHz
    
    // Auto-reload value (will be set per note)
    TIM2->ARR = 1000; // Default 1kHz
    
    // Configure channel 1 for PWM mode 1
    TIM2->CCMR1 |= (0b110 << 4); // PWM mode 1
    TIM2->CCMR1 |= (1 << 3);     // Enable preload
    
    // Set duty cycle to 50% (square wave)
    TIM2->CCR1 = 500; // 50% duty cycle
    
    // Enable channel 1 output
    TIM2->CCER |= (1 << 0);
    
    // Enable auto-reload preload
    TIM2->CR1 |= (1 << 7);
    
    // Start timer
    TIM2->CR1 |= (1 << 0);
}

// Play a note with specified frequency and duration
// Volume is controlled by hardware potentiometer
void play_note(int frequency, int duration_ms) {
    if (frequency == 0) {
        // Rest - stop timer
        TIM2->CR1 &= ~(1 << 0);
        ms_delay(duration_ms);
        return;
    }
    
    // Calculate timer period for desired frequency
    // Timer frequency = 1MHz, so period = 1,000,000 / frequency
    uint32_t period = 1000000 / frequency;
    
    // Update timer period
    TIM2->ARR = period;
    TIM2->CCR1 = period / 2; // 50% duty cycle for square wave
    
    // Restart timer if stopped
    if (!(TIM2->CR1 & (1 << 0))) {
        TIM2->CR1 |= (1 << 0);
    }
    
    // Play for specified duration
    ms_delay(duration_ms);
}
