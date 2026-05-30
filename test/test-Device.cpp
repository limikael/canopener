#include <stdio.h>
#include <cassert>
//#include "canopener.h"
#include "canopener/Device.h"
#include "canopener/MockBus.h"
#include <format>
#include <iostream>
#include <memory>

using namespace canopener;
void test_Device_basic() {
	printf("- Device can be created...\n");
	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);

	device->insert(0x4000,0x03)->setType(Entry::STRING)->setString(std::string("helloxxx"));
	std::string s=device->at(0x4000,0x03)->getString();
	assert(s=="helloxxx");

	device->insert(0x4000,0x04)->setType(Entry::STRING)->setString("hello");
	assert(device->at(0x4000,0x04)->getString()=="hello");

	device->insert(0x4000,0x05)->setType(Entry::INT8)->setInt(123);
	assert(device->at(0x4000,0x05)->getInt()==123);
	assert(device->at(0x4000,0x05)->getFloat()==123.0);
}

void test_Device_expedited_write() {
	printf("- Works with expedited SDO write...\n");
	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);
	device->setNodeId(5);

	device->insert(0x4001,0x33)->setType(Entry::INT32);

	//Write 0x12345678 to index 0x4001, sub-index 0x33
	bus->writeSlcan("t60582301403378563412");
	assert(device->at(0x4001,0x33)->getUint()==0x12345678);
	assert(bus->log[1]=="t585460014033");

	// not for us...
	bus->writeSlcan("t60682301403378563412");
	assert(bus->log.size()==3);

	// non existing...
	bus->writeSlcan("t60582301403478563412");
	assert(bus->log.size()==5);
	assert(bus->log[4]=="t58588001403400000206");

	//for (auto it: bus.log)
	//	std::cout << std::format("{}\n",it);
}

void test_Device_expedited_write16() {
	printf("- Works with expedited SDO write (16bit)...\n");
	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);
	device->setNodeId(5);

	device->insert(0x4001,0x33)->setType(Entry::INT16);

	//Write 0x12345678 to index 0x4001, sub-index 0x33
	bus->writeSlcan("t60582301403378563412");
	//device.loop();
	//printf("%08x\n",device.at(0x4001,0x33).get<uint32_t>());
	assert(device->at(0x4001,0x33)->getInt()==0x5678);
}

void test_Device_expedited_read() {
	printf("- Works with expedited SDO read.....\n");

	std::string s;
	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);
	device->setNodeId(6);

	device->insert(0x2000,0x01)->setType(Entry::UINT32)->setUint(0x12345678);

	// existing
	bus->writeSlcan("t606440002001");
	assert(bus->log.size()==2);
	assert(bus->log[1]=="t58684300200178563412");

	// non-existing
	bus->writeSlcan("t606440012001");
	assert(bus->log[3]=="t58688001200100000206");
}

/*void test_Device_segmented_read() {
	printf("- Works with segmented SDO read.....\n");

	std::shared_ptr<MockBus> bus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(bus);
	device->setNodeId(6);

	device->insert(0x2000,0x01)->setType(Entry::STRING)->setString("hello world");

	cof_t cof;
	cof_init(&cof);
    cof_set(&cof,COF_FUNC,COF_FUNC_SDO_RX);
    cof_set(&cof,COF_NODE_ID,6);
    cof_set(&cof,COF_SDO_CMD,COF_SDO_CMD_UPLOAD);
    cof_set(&cof,COF_SDO_INDEX,0x2000); 
    cof_set(&cof,COF_SDO_SUBINDEX,0x01);
    bus->write(&cof);
}*/
