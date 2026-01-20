#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;
void test_pdo() {
	printf("- pdo subscription...\n");

	MockBus bus;

	Device device(bus);
	device.setNodeId(5);
	device.insert(0x4000,1);

	MasterDevice master(bus);

	RemoteDevice* remote=master.createRemoteDevice(5);
	remote->insert(0x4000,1).subscribe(1);

	device.at(0x4000,1).set(0x12345678);

	for (int i=0; i<10; i++)
		bus.loop();

    /*for (auto it: bus.log)
        std::cout << std::format("{}\n",it);*/

	assert(remote->at(0x4000,1).get<uint32_t>()==0x12345678);
}
