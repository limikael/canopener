#include <stdio.h>
#include <cassert>
#include "canopener/Device.h"
#include "canopener/MasterDevice.h"
#include "canopener/MockBus.h"
#include <format>
#include <iostream>

using namespace canopener;
void test_remote() {
	printf("- MasterDevice and RemoteDevice...\n");

	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);

	device->setNodeId(5);
	device->insert(0x4000,0);
	device->insert(0x4000,1);

	std::shared_ptr<MasterDevice> master=std::make_shared<MasterDevice>(bus);

	std::shared_ptr<RemoteDevice> remote=master->createRemoteDevice(5);
	remote->insert(0x4000,0)->setInt(0x12345678);
	remote->insert(0x4000,1)->setInt(0x55555555);

	for (int i=0; i<10; i++) {
		bus->loop();
	}

	assert(device->at(0x4000,0)->getInt()==0x12345678);
	assert(device->at(0x4000,1)->getInt()==0x55555555);
}

void test_remote_flush() {
	printf("- RemoteDevice can flush...\n");

	auto bus=std::make_shared<MockBus>();
	auto device=std::make_shared<Device>(bus);

	device->setNodeId(5);
	device->insert(0x4000,0);
	device->insert(0x4000,1)->setInt(12345);

	auto master=std::make_shared<MasterDevice>(bus);
	auto remote=master->createRemoteDevice(5);

	bool flushDetected=false;

	remote->insert(0x4000,0)->setInt(0x12345678);
	remote->insert(0x4000,1)->refresh();
	auto promise=remote->flush();
	promise.then([&flushDetected](){
		flushDetected=true;
	});

	for (int i=0; i<10; i++) {
		//printf("loop\n");
		bus->loop();
		if (flushDetected) {
			//printf("flushed\n");
			assert(remote->at(0x4000,1)->getInt()==12345);
		}
	}

	assert(flushDetected);
}

void test_remote_refresh() {
	printf("- Refresh remote device...\n");

	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);

	device->setNodeId(5);
	device->insert(0x4000,0)->setInt(0x12345678);

	auto master=std::make_shared<MasterDevice>(bus);
	auto remote=master->createRemoteDevice(5);
	remote->insert(0x4000,0)->refresh();

	for (int i=0; i<10; i++) {
		bus->loop();
	}

    //for (auto it: bus.log)
    //   std::cout << std::format("{}\n",it);

	assert(remote->at(0x4000,0)->getInt()==0x12345678);
}

void test_remote_segmented_read() {
	printf("- remote segmented SDO read.....\n");

	auto bus=std::make_shared<MockBus>();
	auto device=std::make_shared<Device>(bus);
	auto master=std::make_shared<MasterDevice>(bus);
	auto remote=master->createRemoteDevice(5);

	device->setNodeId(5);
	device->insert(0x2000,0x01)->setType(Entry::STRING)->setString("hello world");
	remote->insert(0x2000,0x01)->setType(Entry::STRING)->refresh();
	auto p=remote->flush();

	for (int i=0; i<10; i++)
		bus->loop();

    for (auto it: bus->log)
       std::cout << std::format("{}\n",it);

	/*assert(p.isResolved());*/

	std::string s=remote->at(0x2000,0x01)->getString();
	printf("read: %s\n",s.c_str());
}