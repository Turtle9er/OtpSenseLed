
class LedControl
{
private:
	int _ledPin;

public:
	void begin(int ledPin);

	void flashError(int longBlinks, int shortBlinks);
	void pulse() { beat(1000, 50); }
	void blink() { beat(100, 100); }
	void burst() { beat(200, 200); }
	void flash() { beat(20, 20); }

	void swap();
	void off();
	void on();

	void beat(uint32_t rate1, uint32_t rate2);
};
extern LedControl Led;
extern LedControl rLed;
extern LedControl gLed;
extern LedControl yLed;
extern LedControl bLed;
extern LedControl gLedGPS;
