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
    // Initialize new variables
    _isNonBlocking = false;
    _ledState = false; // Start in OFF state
    _previousMillis = 0;
    off(); // Start with the LED off
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

// --- NEW: update() Function ---
/**
 * @brief This is the core of the non-blocking flash.
 * Call this in your main loop() to make the LED flash.
 */
void LedControl::update()
{
    // If we're not in a non-blocking mode, do nothing.
    if (!_isNonBlocking) {
        return;
    }

    // Check if it's time to change the state
    unsigned long currentMillis = millis();
    
    // Get the correct duration for the current state (on-time or off-time)
    unsigned long pulseDuration = _ledState ? _rate1 : _rate2;

    if (currentMillis - _previousMillis >= pulseDuration)
    {
        _previousMillis = currentMillis; // Reset the timer
        _ledState = !_ledState;          // Flip the state (ON -> OFF or OFF -> ON)

        // Set the pin to the new physical state
        digitalWrite(_ledPin, _ledState ? _ON : _OFF);
    }
}

// --- NEW: start...() Functions ---
// These set the rates and enable the update() logic.

void LedControl::startPulse()
{
    _rate1 = 1000;
    _rate2 = 50;
    _isNonBlocking = true;
    _previousMillis = millis(); // Start the timer
    _ledState = true;           // Start in the ON state
    digitalWrite(_ledPin, _ON);
}

void LedControl::startBlink()
{
    _rate1 = 100;
    _rate2 = 100;
    _isNonBlocking = true;
    _previousMillis = millis();
    _ledState = true;
    digitalWrite(_ledPin, _ON);
}

void LedControl::startBurst()
{
    _rate1 = 200;
    _rate2 = 200;
    _isNonBlocking = true;
    _previousMillis = millis();
    _ledState = true;
    digitalWrite(_ledPin, _ON);
}

void LedControl::startFlash()
{
    _rate1 = 20;
    _rate2 = 20;
    _isNonBlocking = true;
    _previousMillis = millis();
    _ledState = true;
    digitalWrite(_ledPin, _ON);
}

// --- STANDARD ON/OFF/SWAP FUNCTIONS ---

void LedControl::swap()
{
    // This correctly reads the pin's current state and flips it
    uint8_t state = digitalRead(_ledPin);
    digitalWrite(_ledPin, !state);
}

// --- MODIFIED: on() and off() ---
// These must now disable the non-blocking mode.

void LedControl::off()
{
    _isNonBlocking = false; // Stop any non-blocking flash
    digitalWrite(_ledPin, _OFF);
}

void LedControl::on()
{
    _isNonBlocking = false; // Stop any non-blocking flash
    digitalWrite(_ledPin, _ON);
}