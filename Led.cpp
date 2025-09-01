#include "Arduino.h"
#include "Led.h"

#ifdef INVERSE_SIGNAL
#define _ON LOW
#define _OFF HIGH
#else
#define _ON HIGH
#define _OFF LOW
#endif

LedControl Led;
LedControl rLed;
LedControl gLed;
LedControl yLed;

void LedControl::begin(int ledPin)
{
	_ledPin = ledPin;
	pinMode(ledPin, OUTPUT);
}

void LedControl::beat(uint32_t rate1, uint32_t rate2)
{
	static uint32_t prev = millis();
	uint8_t state = digitalReadFast(_ledPin);
#ifdef INVERSE_SIGNAL
	uint32_t pulse = state ? rate1 : rate2;
#else
	uint32_t pulse = state ? rate2 : rate1;
#endif
	if (millis() - prev > pulse)
	{
		digitalWriteFast(_ledPin, !state);
		prev = millis();
	}
}

void LedControl::swap()
{
	uint8_t state = digitalReadFast(_ledPin);
	digitalWriteFast(_ledPin, !state);
}

void LedControl::off()
{
	digitalWriteFast(_ledPin, _OFF);
}

void LedControl::on()
{
	digitalWriteFast(_ledPin, _ON);
}