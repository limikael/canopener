#pragma once
#include <vector>
#include <functional>

namespace canopener {
    template<typename... Args>
    class Dispatcher {
        std::vector<std::function<void(Args...)>> listeners;

    public:
        void on(std::function<void(Args...)> listener) {
            listeners.push_back(listener);
        }

        void emit(Args... args) {
            for (auto& l: listeners) {
                l(args...);
            }
        }
    };
}