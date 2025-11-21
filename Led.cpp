#include "Arduino.h"
#include "Led.h" // Assuming your header file is Led.h


// --- Private Helper Function: Writes the logical state to the physical pin ---
// This function encapsulates the inversion logic.
// If logicalState is true (ON), it checks 'is_inverted' to decide HIGH or LOW.
void LedControl::_writeHardwareState(bool logicalState) {
    int hardwarePinValue;

    if (logicalState) { // We want the LED to be logically ON
        // If inverted (active-low), ON means pin LOW. Otherwise, pin HIGH.
        hardwarePinValue = is_inverted ? LOW : HIGH;
    } else { // We want the LED to be logically OFF
        // If inverted (active-low), OFF means pin HIGH. Otherwise, pin LOW.
        hardwarePinValue = is_inverted ? HIGH : LOW;
    }

    digitalWrite(_ledPin, hardwarePinValue);
    _ledState = logicalState; // Keep track of the logical state (true=ON, false=OFF)
}


void LedControl::begin(int ledPin, bool inverted)
{
    _ledPin = ledPin;
    is_inverted = inverted; // Store the inversion flag
    pinMode(ledPin, OUTPUT);
    // Initialize non-blocking variables
    _isNonBlocking = false;
    _ledState = false; // Start in logical OFF state
    _previousMillis = 0;
    
    // Call off() which now respects the 'is_inverted' flag
    off(); 
}

/**
 * @brief This is the core BLOCKING beat function.
 * It turns the LED on for rate1 ms, then off for rate2 ms.
 * Now uses on() and off() for inversion awareness.
 */
void LedControl::beat(uint32_t rate1, uint32_t rate2)
{
    // Use the now-fixed on() and off() functions
    on();
    delay(rate1);
    off();
    delay(rate2);
}

// --- DURATION-BASED FUNCTIONS (No changes needed, they call beat/off) ---

void LedControl::pulse(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(1000, 50);
    }
    off();
}

void LedControl::blink(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(100, 100);
    }
    off();
}

void LedControl::burst(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(200, 200);
    }
    off();
}

void LedControl::flash(unsigned long duration_ms) {
    unsigned long startTime = millis();
    while (millis() - startTime < duration_ms) {
        beat(20, 20);
    }
    off();
}

// --- Non-Blocking update() Function ---

void LedControl::update()
{
    if (!_isNonBlocking) {
        return;
    }

    unsigned long currentMillis = millis();
    
    // Get the correct duration for the current state (rate1 for ON, rate2 for OFF)
    unsigned long pulseDuration = _ledState ? _rate1 : _rate2;

    if (currentMillis - _previousMillis >= pulseDuration)
    {
        _previousMillis = currentMillis; 
        
        // Flip the logical state (ON -> OFF or OFF -> ON)
        _ledState = !_ledState;
        
        // Write the new logical state to the pin using the inversion-aware helper
        _writeHardwareState(_ledState);
    }
}

// --- start...() Functions ---
// These set the rates and enable the update() logic.

void LedControl::startPulse()
{
    _rate1 = 1000;
    _rate2 = 50;
    _isNonBlocking = true;
    _previousMillis = millis(); // Start the timer
    
    // We want to START in the ON state
    _writeHardwareState(true);
}

void LedControl::startBlink()
{
    _rate1 = 100;
    _rate2 = 100;
    _isNonBlocking = true;
    _previousMillis = millis();
    
    _writeHardwareState(true);
}

void LedControl::startBurst()
{
    _rate1 = 200;
    _rate2 = 200;
    _isNonBlocking = true;
    _previousMillis = millis();
    
    _writeHardwareState(true);
}

void LedControl::startFlash()
{
    _rate1 = 20;
    _rate2 = 20;
    _isNonBlocking = true;
    _previousMillis = millis();
    
    _writeHardwareState(true);
}

// --- STANDARD ON/OFF/SWAP FUNCTIONS (FIXED) ---

void LedControl::swap()
{
    // Toggle the current stored logical state and write it
    _isNonBlocking = false;
    _writeHardwareState(!_ledState);
}

void LedControl::off()
{
    _isNonBlocking = false; // Stop any non-blocking flash
    // Set logical state to OFF (false)
    _writeHardwareState(false);
}

void LedControl::on()
{
    _isNonBlocking = false; // Stop any non-blocking flash
    // Set logical state to ON (true)
    _writeHardwareState(true);
}