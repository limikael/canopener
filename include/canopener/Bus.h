#pragma once
#include "cof.h"
#include "Dispatcher.h"

namespace canopener {
    class Bus {
    public:
        virtual void write(cof_t *frame)=0;
        virtual void loop() { loopDispatcher.emit(); };
        virtual uint32_t millis()=0; // { return 0; };//=0;

        void writeSlcan(std::string message) {
            cof_t cof;
            cof_from_slcan(&cof,message.c_str());
            write(&cof);
        }

        void notifyMessage(cof_t *cof) {
            messageDispatcher.emit(cof);

            //char cstr[20];
            //::cof_to_slcan(cof,cstr);
            //std::string s(cstr);
            slcanDispatcher.emit(cof_to_slcan_string(cof));
        }

        Dispatcher<> loopDispatcher;
        Dispatcher<cof_t*> messageDispatcher;
        Dispatcher<std::string> slcanDispatcher;
    };
}