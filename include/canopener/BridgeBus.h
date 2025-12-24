#pragma once
#include "Bus.h"
#include <stdio.h>

namespace canopener {
    class BridgeBus: public Bus {
    public:
    	BridgeBus(Bus& a, Bus& b);
        BridgeBus(Bus& a);
        bool available();
        bool read(cof_t *frame);
        void write(cof_t *frame);
        void loop();

    private:
        std::vector<Bus*> busses;
    };
}
