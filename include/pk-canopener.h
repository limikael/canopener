#pragma once
#include "canopener.h"
#include <memory>

extern "C" {
	void canopener_loop();
}

std::shared_ptr<canopener::Bus> getBus();
void initEspBus(int txPin, int rxPin);
