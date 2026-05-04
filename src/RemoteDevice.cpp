#include "canopener/RemoteDevice.h"
#include "canopener/RemoteCmd.h"
#include <stdexcept>

using namespace canopener;

RemoteDevice::RemoteDevice(int nodeId_) { 
	nodeId=nodeId_; 
	/*insert(0x1A00,1); // REMOVE
	insert(0x1A01,1);
	insert(0x1A02,1);
	insert(0x1A03,1);*/
}

void RemoteDevice::handleChange(std::shared_ptr<Entry> e) {
	//printf("change detected in remote device!\n");
	auto c=std::make_shared<RemoteCmd>(RemoteCmd::SDO_WRITE,e);
	c->setRemoteDevice(this);
	cmds.push_back(c);
}

void RemoteDevice::handleRefresh(std::shared_ptr<Entry> e) {
	//printf("refresh detected in remote device!\n");
	auto c=std::make_shared<RemoteCmd>(RemoteCmd::SDO_READ,e);
	c->setRemoteDevice(this);
	cmds.push_back(c);
}

std::shared_ptr<FlushPromise> RemoteDevice::flush() {
	auto c=std::make_shared<RemoteCmd>(RemoteCmd::FLUSH);
	c->setRemoteDevice(this);
	cmds.push_back(c);
	return c->getFlushPromise();
}

void RemoteDevice::handleMessage(cof_t *frame) {
	if (cmdInFlight)
		cmdInFlight->handleMessage(frame);

	/*for (int pdoIndex=0; pdoIndex<4; pdoIndex++) {
		int pdoId=0x180+(pdoIndex*0x100)+getNodeId();
		if (frame->id==pdoId) {

			Entry& pdoEntry=at(0x1A00+pdoIndex,1);
			uint8_t bits=pdoEntry.getData(0);
			uint8_t subindex=pdoEntry.getData(1);
			uint16_t index=pdoEntry.getData(2)+(pdoEntry.getData(3)<<8);

			//printf("got pdo update, %04x:%02x ...\n",index,subindex);
			Entry& entry=at(index,subindex);

			entry.setData(0,frame->data[0]);
			entry.setData(1,frame->data[1]);
			entry.setData(2,frame->data[2]);
			entry.setData(3,frame->data[3]);

			entry.clearDirty();

			entry.changeDispatcher.emit();

			//entry.emit("change");
		}
	}*/
}

void RemoteDevice::handleLoop() {
	if (!cmdInFlight && cmds.size()) {
		cmdInFlight=cmds.front();
		cmds.erase(cmds.begin());
	}

	if (cmdInFlight) {
		cmdInFlight->handleLoop();
		if (cmdInFlight->isComplete())
			cmdInFlight=nullptr;
	}
}

std::shared_ptr<Bus> RemoteDevice::getBus() { 
	return masterDevice->getBus(); 
}

void RemoteDevice::setMasterDevice(MasterDevice *masterDevice_) { 
	if (masterDevice)
		throw std::runtime_error("can't change master device");

	masterDevice=masterDevice_; 

	getBus()->loopDispatcher.on([this]() {
		handleLoop();
	});

	getBus()->messageDispatcher.on([this](cof_t *frame) {
		handleMessage(frame);
	});
}

