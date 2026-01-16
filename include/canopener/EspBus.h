#pragma once
#include "Bus.h"
#include <stdio.h>

#ifdef ESP32
#include <Arduino.h>

namespace canopener {
    class EspBus: public Bus {
    public:
    	EspBus(int txPin_, int rxPin_);
        void write(cof_t *frame);
        void loop();
        uint32_t millis() { 
            /*if (::millis==nullptr)
                return 0;*/

            return ::millis();
        }

        bool available();
        bool read(cof_t *frame);

    private:
        int txPin, rxPin;
        uint32_t lastBusTime;
        bool initialized;
        void resetBus();
        int sendErrorCount;

        void populatePeeked();
        cof_t peeked;
        bool havePeeked;
    };
}

#endif