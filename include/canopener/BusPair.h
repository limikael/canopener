#pragma once
#include "Bus.h"

namespace canopener {
    class PairBus: public Bus {
    public:
        virtual void write(cof_t *frame) override {
            other.lock()->notifyMessage(frame);
        }

        virtual uint32_t millis() override { return 0; }
        void setOther(std::weak_ptr<PairBus> other_) { other=other_; }

    private:
        std::weak_ptr<PairBus> other;
    };

    class BusPair {
    public:
        BusPair() {
            first=std::make_shared<PairBus>();
            second=std::make_shared<PairBus>();
            first->setOther(second);
            second->setOther(first);
        }

        std::shared_ptr<Bus> getFirst() { return first; }
        std::shared_ptr<Bus> getSecond() { return second; }

    private:
        std::shared_ptr<PairBus> first, second;
    };
}