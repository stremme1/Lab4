// fur_elise_player_with_pot.c
// Complete Für Elise Player for STM32L432KC with Potentiometer Control
// Based on tutorial clock configuration solution
// Potentiometer on PA0 controls volume (PWM duty cycle)

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"

// External reference to the note data from lab4_starter.c
extern const int notes[][2];

// Function prototypes
void TIM2_Init(void);
void ADC_Init(void);
uint16_t ADC_Read(void);
void play_note(int frequency, int duration_ms);
void ms_delay(int ms);

// Define pins
#define AUDIO_PIN 5   // PA5 for TIM2_CH1
#define POT_PIN 0     // PA0 for ADC

// ADC definitions
#define ADC_BASE (0x50000000UL)
#define ADC1 ((ADC_TypeDef *) ADC_BASE)

typedef struct {
    __IO uint32_t ISR;       // ADC interrupt and status register
    __IO uint32_t IER;       // ADC interrupt enable register
    __IO uint32_t CR;        // ADC control register
    __IO uint32_t CFGR;      // ADC configuration register
    __IO uint32_t CFGR2;     // ADC configuration register 2
    __IO uint32_t SMPR1;     // ADC sampling time register 1
    __IO uint32_t SMPR2;     // ADC sampling time register 2
    __IO uint32_t TR1;       // ADC watchdog threshold register 1
    __IO uint32_t TR2;       // ADC watchdog threshold register 2
    __IO uint32_t TR3;       // ADC watchdog threshold register 3
    __IO uint32_t SQR1;      // ADC regular sequence register 1
    __IO uint32_t SQR2;      // ADC regular sequence register 2
    __IO uint32_t SQR3;      // ADC regular sequence register 3
    __IO uint32_t SQR4;      // ADC regular sequence register 4
    __IO uint32_t DR;        // ADC regular data register
    __IO uint32_t JSQR;      // ADC injected sequence register
    __IO uint32_t OFR1;      // ADC offset register 1
    __IO uint32_t OFR2;      // ADC offset register 2
    __IO uint32_t OFR3;      // ADC offset register 3
    __IO uint32_t OFR4;      // ADC offset register 4
    __IO uint32_t JDR1;      // ADC injected data register 1
    __IO uint32_t JDR2;      // ADC injected data register 2
    __IO uint32_t JDR3;      // ADC injected data register 3
    __IO uint32_t JDR4;      // ADC injected data register 4
    __IO uint32_t AWD2CR;    // ADC analog watchdog 2 configuration register
    __IO uint32_t AWD3CR;    // ADC analog watchdog 3 configuration register
    __IO uint32_t DIFSEL;    // ADC differential mode selection register
    __IO uint32_t CALFACT;   // ADC calibration factors register
} ADC_TypeDef;

// Main function - plays Für Elise with potentiometer volume control
int main(void) {
    // Configure flash to add waitstates to avoid timing errors
    configureFlash();

    // Setup the PLL and switch clock source to the PLL
    configureClock();

    // Turn on clock to GPIOA
    RCC->AHB2ENR |= (1 << 0);
    
    // Turn on clock to ADC1
    RCC->AHB2ENR |= (1 << 13);

    // Set AUDIO_PIN as alternate function
    pinMode(AUDIO_PIN, GPIO_ALT);
    
    // Set POT_PIN as analog input
    pinMode(POT_PIN, GPIO_ANALOG);
    
    // Initialize ADC for potentiometer
    ADC_Init();
    
    // Initialize timer for PWM generation
    TIM2_Init();
    
    // Play Für Elise with potentiometer control
    int i = 0;
    while (notes[i][1] != 0) { // Loop until duration is 0 (end marker)
        if (notes[i][0] == 0) {
            // Rest - just delay
            ms_delay(notes[i][1]);
        } else {
            // Play note with potentiometer-controlled volume
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

// Initialize ADC for potentiometer reading
void ADC_Init(void) {
    // Enable ADC
    ADC1->CR |= (1 << 0);
    
    // Wait for ADC to be ready
    while (!(ADC1->ISR & (1 << 0)));
    
    // Configure ADC for single conversion
    ADC1->CFGR &= ~(1 << 13); // Single conversion mode
    
    // Set channel 1 (PA0) as first in sequence
    ADC1->SQR1 = (1 << 6); // 1 conversion in sequence
    
    // Set sampling time for channel 1
    ADC1->SMPR1 = (0b111 << 0); // 247.5 cycles sampling time
}

// Read ADC value from potentiometer
uint16_t ADC_Read(void) {
    // Start conversion
    ADC1->CR |= (1 << 2);
    
    // Wait for conversion to complete
    while (!(ADC1->ISR & (1 << 2)));
    
    // Return ADC value
    return ADC1->DR;
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
void play_note(int frequency, int duration_ms) {
    if (frequency == 0) {
        // Rest - stop timer
        TIM2->CR1 &= ~(1 << 0);
        ms_delay(duration_ms);
        return;
    }
    
    // Read potentiometer value (0-4095)
    uint16_t pot_value = ADC_Read();
    
    // Calculate timer period for desired frequency
    // Timer frequency = 1MHz, so period = 1,000,000 / frequency
    uint32_t period = 1000000 / frequency;
    
    // Update timer period
    TIM2->ARR = period;
    
    // Calculate duty cycle based on potentiometer
    // pot_value ranges from 0-4095, map to 10%-90% duty cycle
    uint32_t duty_cycle = (period * (100 + (pot_value * 80) / 4095)) / 1000;
    if (duty_cycle > period) duty_cycle = period;
    if (duty_cycle < period/10) duty_cycle = period/10;
    
    TIM2->CCR1 = duty_cycle;
    
    // Restart timer if stopped
    if (!(TIM2->CR1 & (1 << 0))) {
        TIM2->CR1 |= (1 << 0);
    }
    
    // Play for specified duration
    ms_delay(duration_ms);
}
