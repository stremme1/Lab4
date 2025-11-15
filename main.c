// main.c
// DAC-based Music Player for STM32L432KC
//
// Author: Emmett Stralka
// Email: estralka@hmc.edu
// Date: 9/29/25
//
// Description: Simple DAC-based music player with a basic tune for testing

#include "STM32L432KC_RCC.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_DAC.h"
#include "STM32L432KC_TIMER.h"  // For ms_delay

// Twinkle Twinkle Little Star - Original frequencies in C major
// Format: {frequency in Hz, duration in ms}
// Standard musical note frequencies (A4 = 440 Hz)
const int simple_tune[][2] = {
    // Twinkle, twinkle, little star
    {262, 300},  // C4 (Middle C)
    {262, 300},  // C4
    {392, 300},  // G4
    {392, 300},  // G4
    {440, 300},  // A4
    {440, 300},  // A4
    {392, 600},  // G4 (held)
    {0, 50},     // Rest
    
    // How I wonder what you are
    {349, 300},  // F4
    {349, 300},  // F4
    {330, 300},  // E4
    {330, 300},  // E4
    {294, 300},  // D4
    {294, 300},  // D4
    {262, 600},  // C4 (held)
    {0, 50},     // Rest
    
    // Up above the world so high
    {392, 300},  // G4
    {392, 300},  // G4
    {349, 300},  // F4
    {349, 300},  // F4
    {330, 300},  // E4
    {330, 300},  // E4
    {294, 600},  // D4 (held)
    {0, 50},     // Rest
    
    // Like a diamond in the sky
    {392, 300},  // G4
    {392, 300},  // G4
    {349, 300},  // F4
    {349, 300},  // F4
    {330, 300},  // E4
    {330, 300},  // E4
    {294, 600},  // D4 (held)
    {0, 50},     // Rest
    
    // Twinkle, twinkle, little star
    {262, 300},  // C4
    {262, 300},  // C4
    {392, 300},  // G4
    {392, 300},  // G4
    {440, 300},  // A4
    {440, 300},  // A4
    {392, 600},  // G4 (held)
    {0, 50},     // Rest
    
    // How I wonder what you are
    {349, 300},  // F4
    {349, 300},  // F4
    {330, 300},  // E4
    {330, 300},  // E4
    {294, 300},  // D4
    {294, 300},  // D4
    {262, 800},  // C4 (held longer - end of song)
    {0, 200},    // Rest before repeat
    {0, 0}       // End marker
};

// Sample rate for audio generation (Hz)
#define SAMPLE_RATE 48000  // 48 kHz - even higher sample rate for maximum resolution

// Function to play a note using DAC
void play_note(int frequency, int duration_ms) {
    // Use DAC to play sine wave (envelope is handled inside)
    DAC_PlaySineWave((float)frequency, duration_ms, SAMPLE_RATE);
    // No extra delay needed - envelope handles smooth transitions
}

// Main function
int main(void) {
    // Initialize system
    configureFlash();
    configureClock();
    
    // Initialize DAC for audio output (using channel 1 on PA4)
    DAC_InitAudio(DAC_CHANNEL_1);
    
    // CRITICAL TEST: Direct register write test
    // This bypasses all functions to test if the DAC hardware works at all
    // Measure PA4 with multimeter - you should see voltage changes
    
    // Test 1: Maximum value (4095) - should be ~3.1V
    DAC->DHR12R1 = 4095;
    ms_delay(2000);  // Hold for 2 seconds
    
    // Test 2: Mid-point (2048) - should be ~1.65V  
    DAC->DHR12R1 = 2048;
    ms_delay(2000);  // Hold for 2 seconds
    
    // Test 3: Quarter (1024) - should be ~0.825V
    DAC->DHR12R1 = 1024;
    ms_delay(2000);  // Hold for 2 seconds
    
    // Test 4: Minimum (0) - should be ~0.2V (buffer minimum)
    DAC->DHR12R1 = 0;
    ms_delay(2000);  // Hold for 2 seconds
    
    // Test 5: Back to maximum
    DAC->DHR12R1 = 4095;
    ms_delay(2000);  // Hold for 2 seconds
    
    // If you're still seeing 0.2V for all values, the DAC is not working
    // Possible causes:
    // 1. VDDA is not 3.3V (measure VDDA pin)
    // 2. VREF+ is not actually connected (double-check your bridge)
    // 3. DAC hardware is damaged
    // 4. PA4 is shorted or has a load pulling it down
    
    // Play the simple tune in a loop
    while(1) {
        int i = 0;
        while (simple_tune[i][1] != 0) {
            play_note(simple_tune[i][0], simple_tune[i][1]);
            i++;
        }
        // Short pause between repetitions
        ms_delay(200);
    }
    
    return 0;
}
