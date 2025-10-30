#include "Arduino.h"
#include "Led.h" // Assuming your header file is Led.h

// --- This block defines ON and OFF based on your INVERSE_SIGNAL ---
// This is great, we'll use it everywhere.
#ifdef INVERSE_SIGNAL
#define _ON LOW
#define _OFF HIGH
#else
#define _ON HIGH
#define _OFF LOW
#endif

// --- DELETED ---
// All the global LedControl objects (rLed, gLed, etc.)
// have been removed. You must add them to your main .ino file.

void LedControl::begin(int ledPin)
{
    _ledPin = ledPin;
    pinMode(ledPin, OUTPUT);
    off(); // Start with the LED off
}

void LedControl::flashError(int longBlinks, int shortBlinks)
{
    // Updated to use the _ON and _OFF macros
    for (int i = 0; i < longBlinks; i++)
    {
        digitalWrite(_ledPin, _ON);
        delay(500);
        digitalWrite(_ledPin, _OFF);
        delay(500);
    }
    delay(1000);
    for (int i = 0; i < shortBlinks; i++)
    {
        digitalWrite(_ledPin, _ON);
        delay(150);
        digitalWrite(_ledPin, _OFF);
        delay(150);
    }
    off(); // Ensure it's off when done
}

/**
 * @brief This is the core BLOCKING beat function.
 * It turns the LED on for rate1 ms, then off for rate2 ms.
 * The non-blocking version was removed to prevent a compiler error.
 */
void LedControl::beat(uint32_t rate1, uint32_t rate2)
{
    digitalWrite(_ledPin, _ON);
    delay(rate1);
    digitalWrite(_ledPin, _OFF);
    delay(rate2);
}

// --- NEW DURATION-BASED FUNCTIONS ---

/**
 * @brief Keeps pulsing (1000ms on, 50ms off) for a set duration.
 */
void LedControl::pulse(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(1000, 50); // Pulse rates
    }
    off(); // Ensure the LED is off when finished
}

/**
 * @brief Keeps blinking (100ms on, 100ms off) for a set duration.
 */
void LedControl::blink(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(100, 100); // Blink rates
    }
    off();
}

/**
 * @brief Keeps bursting (200ms on, 200ms off) for a set duration.
 */
void LedControl::burst(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(200, 200); // Burst rates
    }
    off();
}

/**
 * @brief Keeps flashing (20ms on, 20ms off) for a set duration.
 */
void LedControl::flash(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(20, 20); // Flash rates
    }
    off();
}

// --- STANDARD ON/OFF/SWAP FUNCTIONS ---

void LedControl::swap()
{
    // This correctly reads the pin's current state and flips it
    uint8_t state = digitalRead(_ledPin);
    digitalWrite(_ledPin, !state);
}

void LedControl::off()
{
    digitalWrite(_ledPin, _OFF);
}

void LedControl::on()
{
    digitalWrite(_ledPin, _ON);
}