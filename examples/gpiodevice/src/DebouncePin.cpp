#include "DebouncePin.h"

DebouncePin::DebouncePin(
    uint8_t pin,
    bool pull,
    unsigned long debounceMs
)
    : _pin(pin),
      _debounceMs(debounceMs),
      _changed(false),
      _lastChangeTime(0)
{
    pinMode(pin, pull ? INPUT_PULLDOWN : INPUT);

    _lastRaw = readRaw();
    _value   = _lastRaw;
}

bool DebouncePin::readRaw() const {
    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += digitalRead(_pin);
        delayMicroseconds(500);
    }
    return sum >= 4;  // 4/5 wins

    // If using INPUT_PULLUP, LOW = pressed
//    return digitalRead(_pin) == HIGH;
}

void DebouncePin::update() {
    bool raw = readRaw();
    unsigned long now = millis();

    if (raw != _lastRaw) {
        _lastRaw = raw;

        if (now - _lastChangeTime >= _debounceMs) {
            if (raw != _value) {
                _value = raw;
                _changed = true;
                _lastChangeTime = now;
            }
        }
    }
}

bool DebouncePin::getValue() const {
    return _value;
}

bool DebouncePin::didChange() {
    update();

    if (_changed) {
        _changed = false;
        return true;
    }
    return false;
}
