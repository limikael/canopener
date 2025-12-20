#include "canopener.h"

using namespace canopener;

BridgeBus::BridgeBus(Bus& a, Bus &b) {
	busses.push_back(&a);
	busses.push_back(&b);
}

bool BridgeBus::available() {
	for (Bus* bus: busses)
		if (bus->available())
			return true;

	return false;
}

bool BridgeBus::read(cof_t *frame) {
	for (Bus* outerBus: busses) {
		if (outerBus->available()) {
			outerBus->read(frame);
			for (Bus* innerBus: busses) {
				if (innerBus!=outerBus)
					innerBus->write(frame);
			}
			return true;
		}
	}

	return false;
}

void BridgeBus::write(cof_t *frame) {
	for (Bus* bus: busses)
		bus->write(frame);
}

void BridgeBus::loop() {
	for (Bus* bus: busses)
		bus->loop();
}
