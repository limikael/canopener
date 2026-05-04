#include <stdio.h>
#include <cassert>
#include "canopener/EntryContainer.h"
#include "canopener/Entry.h"
#include <format>
#include <iostream>
#include <memory>

using namespace canopener;
void test_EntryContainer_basic() {
	printf("- EntryContainer can be created...\n");
	std::shared_ptr<EntryContainer> container=std::make_shared<EntryContainer>();

	//container->insert(0x4000,0x03)->setType(Entry::STRING)->setString("helloxxx");

	container->insert(0x4000,0x03);
	container->at(0x4000,0x03)->setType(Entry::STRING)->setString("helloxxx");
	assert(container->at(0x4000,0x03)->getString()=="helloxxx");

	container->insert(0x4000,0x05)->setType(Entry::INT8)->setInt(123);
	assert(container->at(0x4000,0x05)->getInt()==123);

	container->insert(0x4000,0x06)->setType(Entry::INT8)->setFloat(123.567);
	assert(container->at(0x4000,0x06)->getFloat()==123);

	container->insert(0x4000,0x00)->setType(Entry::UINT32)->setInt(123);
	assert(container->at(0x4000,0x00)->getInt()==123);
	assert(container->at(0x4000,0x00)->getString()=="123");
	//assert(device.at(0x4000).get<uint32_t>()==123);
}

void test_EntryContainer_notifications() {
	printf("- EntryContainer can notify...\n");
	std::shared_ptr<EntryContainer> container=std::make_shared<EntryContainer>();

	auto e=container->insert(0x4000,0x03);

	e->setInt(123);
}