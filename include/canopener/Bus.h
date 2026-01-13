#pragma once
#include "cof.h"
#include "Dispatcher.h"

namespace canopener {
    class Bus {
    public:
        virtual bool available()=0;
        virtual bool read(cof_t *frame)=0;
        virtual void write(cof_t *frame)=0;
        virtual void loop() { loopDispatcher.emit(); };
        virtual uint32_t millis()=0; // { return 0; };//=0;

        Dispatcher<> loopDispatcher;
    };
}