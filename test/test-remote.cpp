#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;
void test_remote() {
	printf("- MasterDevice and RemoteDevice...\n");

	MockBus bus;

	Device device(bus);
	device.setNodeId(5);
	device.insert(0x4000,0);
	device.insert(0x4000,1);

	MasterDevice master(bus);

	RemoteDevice* remote=master.createRemoteDevice(5);
	remote->insert(0x4000,0).set(0x12345678);
	remote->insert(0x4000,1).set(0x55555555);

	for (int i=0; i<10; i++) {
		bus.loop();
	}

	assert(device.at(0x4000,0).get<uint32_t>()==0x12345678);
	assert(device.at(0x4000,1).get<uint32_t>()==0x55555555);
}

void test_remote_generations() {
	printf("- RemoteDevice tracks genertion...\n");

	MockBus bus;

	Device device(bus);
	device.setNodeId(5);
	device.insert(0x4000,0);
	device.insert(0x4000,1);

	MasterDevice master(bus);

	RemoteDevice* remote=master.createRemoteDevice(5);
	int listenerCalled=0;
	int listenHandle=remote->commitGenerationChangeDispatcher.on([&listenerCalled]() {
		//printf("change!!!\n");
		listenerCalled++;
	});

	assert(remote->getGeneration()==0);

	remote->insert(0x4000,0).set(0x12345678);
	assert(remote->getGeneration()==1);
	assert(remote->getCommitGeneration()==0);

	for (int i=0; i<10; i++) {
		bus.loop();
	}

	assert(remote->getGeneration()==1);
	assert(remote->getCommitGeneration()==1);

	remote->commitGenerationChangeDispatcher.off(listenHandle);

	remote->at(0x4000,0).set(0x11111111);
	assert(remote->getGeneration()==2);

	remote->insert(0x4000,1).set(0x55555555);

	for (int i=0; i<10; i++) {
		bus.loop();
	}

	assert(listenerCalled==1);

    /*for (auto it: bus.log)
        std::cout << std::format("{}\n",it);*/


	/*assert(device.at(0x4000,0).get<uint32_t>()==0x12345678);
	assert(device.at(0x4000,1).get<uint32_t>()==0x55555555);*/
}
