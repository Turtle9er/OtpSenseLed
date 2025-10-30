#ifndef Led_h
#define Led_h

#include "Arduino.h"

class LedControl
{
private:
    int _ledPin;

public:
    void begin(int ledPin);

    // --- Blocking Flash Functions ---
    
    // Flashes a two-part error code (e.g., 3 long, 2 short)
    void flashError(int longBlinks, int shortBlinks);
    
    // Flashes for a specific duration
    void pulse(unsigned long duration_ms); // 1000ms on, 50ms off
    void blink(unsigned long duration_ms); // 100ms on, 100ms off
    void burst(unsigned long duration_ms); // 200ms on, 200ms off
    void flash(unsigned long duration_ms); // 20ms on, 20ms off

    // --- Basic Controls ---
    void swap(); // Toggles the current state
    void off();  // Turns LED off (respects INVERSE_SIGNAL)
    void on();   // Turns LED on (respects INVERSE_SIGNAL)

    // --- Core blocking beat ---
    // This is the function all the duration-based ones call
    void beat(uint32_t rate1, uint32_t rate2);
};

#endif