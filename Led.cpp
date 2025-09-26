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
LedControl bLed;
LedControl gLedGPS;

void LedControl::begin(int ledPin)
{
	_ledPin = ledPin;
	pinMode(ledPin, OUTPUT);
}

void LedControl::flashError(int longBlinks, int shortBlinks)
{
	for (int i = 0; i < longBlinks; i++)
	{
		digitalWriteFast(_ledPin, HIGH);
		delay(500);
		digitalWriteFast(_ledPin, LOW);
		delay(500);
	}
	delay(1000);
	for (int i = 0; i < shortBlinks; i++)
	{
		digitalWriteFast(_ledPin, HIGH);
		delay(150);
		digitalWriteFast(_ledPin, LOW);
		delay(150);
	}
}

void LedControl::beat(uint32_t rate1, uint32_t rate2)
{
	static uint32_t prev = millis();
	uint8_t state = digitalRead(_ledPin);
#ifdef INVERSE_SIGNAL
	uint32_t pulse = state ? rate1 : rate2;
#else
	uint32_t pulse = state ? rate2 : rate1;
#endif
	if (millis() - prev > pulse)
	{
		digitalWrite(_ledPin, !state);
		prev = millis();
	}
}

void LedControl::swap()
{
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
