#include <stdio.h>
#include <cassert>
#include "canopener/Device.h"
#include "canopener/MockBus.h"
#include "canopener/MasterDevice.h"
#include <format>
#include <iostream>
#include <memory>

using namespace canopener;
void test_MasterDevice_connectedDeviceIds() {
	printf("- MasterDevice can return connected device ids...\n");
	std::shared_ptr<MockBus> mockBus=std::make_shared<MockBus>();
	std::shared_ptr<Device> device=std::make_shared<Device>(mockBus);
	device->setNodeId(55);
	std::shared_ptr<MasterDevice> master=std::make_shared<MasterDevice>(mockBus);

    for (int i=0; i<10; i++) {
        mockBus->mockMillis+=1000;
        mockBus->loop();
    }

    auto a=master->getConnectedDeviceIds();
    assert(a->size()==1);
    assert(a->at(0)==55);

    device=nullptr;

    for (int i=0; i<10; i++) {
        mockBus->mockMillis+=1000;
        mockBus->loop();
    }

    a=master->getConnectedDeviceIds();
    assert(a->size()==0);

    /*for (int i=0; i<mockBus->log.size(); i++)
        std::cout << mockBus->log[i] << std::endl;*/
}

