#include "Blinker.h"

Blinker::Blinker(uint8_t pin_, int period_) {
	pin=pin_;
	period=period_;
	deadline=0;
	index=0;

	pinMode(pin,OUTPUT);
}

void Blinker::setPattern(std::string pattern_) {
	pattern=pattern_;
}

void Blinker::loop() {
	uint32_t now=millis();
	if (now>deadline) {
		deadline=now+period;
		index++;

		if (index>=pattern.length())
			index=0;

		digitalWrite(pin,pattern[index]==' ');
	}
}
