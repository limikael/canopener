#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;

void test_pdo() {
	printf("- pdo subscription...\n");

	auto bus=std::make_shared<MockBus>();
	auto device=std::make_shared<Device>(bus);

	device->setNodeId(5);
	device->insert(0x4000,1);

	auto master=std::make_shared<MasterDevice>(bus);
	auto remote=master->createRemoteDevice(5);

	remote->insert(0x4000,1)->subscribe(1);
	for (int i=0; i<20; i++)
		bus->loop();

	device->at(0x4000,1)->setInt(0x12345678);

	for (int i=0; i<20; i++)
		bus->loop();

    /*for (auto it: bus.log)
        std::cout << std::format("{}\n",it);*/

	//printf("it is: %d\n",remote->at(0x4000,1)->getInt());
	assert(remote->at(0x4000,1)->getInt()==0x12345678);
}

void test_pdo_inhibit() {
	printf("- pdo inhibit...\n");

	auto bus=std::make_shared<MockBus>();
	auto device=std::make_shared<Device>(bus);
	auto master=std::make_shared<MasterDevice>(bus);
	auto remote=master->createRemoteDevice(5);

	device->setNodeId(5);
	device->insert(0x4000,1);
	remote->insert(0x4000,1)->subscribe(1);
	for (int i=0; i<100; i++)
		bus->loop();

	//device->at(0x4000,1)->setInt(123);
	/*for (int i=0; i<20; i++)
		bus->loop();

	//assert(remote->at(0x4000,1)->getInt()==123);*/

    /*for (auto it: bus->log)
        std::cout << std::format("{}\n",it);*/
}
