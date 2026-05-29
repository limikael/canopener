#pragma once
#include "cof.h"
#include "Dispatcher.h"

namespace canopener {
    class LoopTimeoutEvent {
    public:
        int getTimeout() { return timeout; }
        void setTimeout(int t) { 
            if (t<0) 
                t=0;

            if (timeout<0 || t<timeout)
                timeout=t;
        }

        int timeout=-1;
    };

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

        void writeData(int id, std::vector<uint8_t> data) {
            cof_t cof;

            cof.id=id;
            cof.len=data.size();
            for (int i=0; i<data.size(); i++)
                cof.data[i]=data[i];

            write(&cof);
        }

        void notifyMessage(cof_t *cof) {
            messageDispatcher.emit(cof);
            slcanDispatcher.emit(cof_to_slcan_string(cof));

            std::vector<uint8_t> v(cof->len);
            for (int i=0; i<cof->len; i++)
                v[i]=cof->data[i];

            dataDispatcher.emit(cof->id,v);
        }

        int getTimeout() {
            auto ev=std::make_shared<LoopTimeoutEvent>();
            loopTimeoutDispatcher.emit(ev);

            return ev->getTimeout();
        }

        Dispatcher<> loopDispatcher;
        Dispatcher<std::shared_ptr<LoopTimeoutEvent>> loopTimeoutDispatcher;
        Dispatcher<cof_t*> messageDispatcher;
        Dispatcher<std::string> slcanDispatcher;
        Dispatcher<int, std::vector<uint8_t>> dataDispatcher;
    };
}