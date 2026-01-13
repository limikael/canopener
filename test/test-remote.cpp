#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;
void test_remote() {
	printf("- MasterDevice and RemoteDevice...\n");

	BusHub hub;
	Bus* masterBus=hub.createBus();
	Bus* deviceBus=hub.createBus();
	Bus* observerBus=hub.createBus();

	Device device(*deviceBus);
	device.setNodeId(5);
	device.insert(0x4000,0);
	device.insert(0x4000,1);

	MasterDevice master(*masterBus);

	RemoteDevice* remote=master.createRemoteDevice(5);
	remote->insert(0x4000,0).set(0x12345678);
	remote->insert(0x4000,1).set(0x55555555);

	for (int i=0; i<10; i++) {
		device.loop();

		//masterBus->loop();
		master.loop();
//		remote->loop();
	}

	while (observerBus->available()) {
		cof_t c;
		observerBus->read(&c);
		//std::cout << std::format("frame: {}\n",cof_to_slcan_string(&c));
	}

	assert(device.at(0x4000,0).get<uint32_t>()==0x12345678);
	assert(device.at(0x4000,1).get<uint32_t>()==0x55555555);
}
