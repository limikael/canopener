#ifdef PEAKERNEL

#include "pk-canopener.h"
#include "canopener/EspBus.h"

using namespace canopener;

static std::shared_ptr<Bus> bus;

void initEspBus(int txPin, int rxPin) {
	if (!bus)
		bus=std::make_shared<EspBus>(txPin,rxPin);
}

std::shared_ptr<Bus> getBus() {
	return bus;
}

void canopener_loop() {
	if (bus)
		bus->loop();
}

#endif