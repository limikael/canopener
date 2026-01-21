#pragma once
#include <Arduino.h>

class Blinker {
public:
    Blinker(uint8_t pin_, int period=100);
    void setPattern(std::string pattern_);
    void loop();

private:
    int index;
    uint32_t deadline;
    uint8_t pin;
    int period;
    std::string pattern;
};
