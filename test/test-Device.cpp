#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;
void test_Device_basic() {
	printf("- Device can be created...\n");
	MockBus bus;
	Device device(bus);

	device.insert(0x4000,0x03).setType(Entry::STRING).set(std::string("helloxxx"));
	std::string s=device.at(0x4000,0x03).get<std::string>();
	assert(s=="helloxxx");

	// Doesn't work for now...
	device.insert(0x4000,0x04).setType(Entry::STRING).set("hello");
	assert(device.at(0x4000,0x04).get<std::string>()=="hello");

	//assert(device.at(0x4000,0x03).get<std::string>()==std::string("hello"));

	device.insert(0x4000,0x05).setType(Entry::INT8).set(123);
	assert(device.at(0x4000,0x05).get<int8_t>()==123);
	assert(device.at(0x4000,0x05).get<uint32_t>()==123);
	assert(device.at(0x4000,0x05).get<float>()==123.0);

	device.at(0x4000,0x05).set("111");
	assert(device.at(0x4000,0x05).get<int>()==111);

	device.insert(0x4000,0x00).setType(Entry::UINT32).set(123);
	assert(device.at(0x4000,0x00).get<int>()==123);
	//assert(device.at(0x4000).get<uint32_t>()==123);
}

void test_Device_expedited_write() {
	printf("- Works with expedited SDO write...\n");
	MockBus bus;
	Device device(bus);
	device.setNodeId(5);

	device.insert(0x4001,0x33).setType(Entry::INT32);

	//Write 0x12345678 to index 0x4001, sub-index 0x33
	bus.writeSlcan("t60582301403378563412");
	assert(device.at(0x4001,0x33).get<uint32_t>()==0x12345678);
	assert(bus.log[1]=="t585460014033");

	// not for us...
	bus.writeSlcan("t60682301403378563412");
	assert(bus.log.size()==3);

	// non existing...
	bus.writeSlcan("t60582301403478563412");
	assert(bus.log.size()==5);
	assert(bus.log[4]=="t58588001403400000206");

	/*for (auto it: bus.log)
		std::cout << std::format("{}\n",it);*/
}

void test_Device_expedited_write16() {
	printf("- Works with expedited SDO write (16bit)...\n");
	MockBus bus;
	Device device(bus);
	device.setNodeId(5);

	device.insert(0x4001,0x33).setType(Entry::INT16);

	//Write 0x12345678 to index 0x4001, sub-index 0x33
	bus.writeSlcan("t60582301403378563412");
	//device.loop();
	//printf("%08x\n",device.at(0x4001,0x33).get<uint32_t>());
	assert(device.at(0x4001,0x33).get<uint32_t>()==0x5678);
}

void test_Device_expedited_read() {
	printf("- Works with expedited SDO read.....\n");

	std::string s;
	MockBus bus;
	Device device(bus);
	device.setNodeId(6);

	device.insert(0x2000,0x01).setType(Entry::UINT32).set(0x12345678);

	// existing
	bus.writeSlcan("t606440002001");
	assert(bus.log[1]=="t58684300200178563412");

	// non-existing
	bus.writeSlcan("t606440012001");
	assert(bus.log[3]=="t58688001200100000206");
}
