#include "canopener/RemoteDevice.h"
#include "canopener/RemoteCmd.h"
#include "canopener/Pdo.h"
#include <stdexcept>

using namespace canopener;

RemoteDevice::RemoteDevice(int nodeId_) { 
	nodeId=nodeId_; 
	state=DISCONNECTED;
}

RemoteDevice::~RemoteDevice() {
	if (loopHandlerId)
		getBus()->loopDispatcher.off(loopHandlerId);

	if (messageHandlerId)
		getBus()->messageDispatcher.off(messageHandlerId);
}

void RemoteDevice::handleChange(std::shared_ptr<Entry> e) {
	//printf("change detected in remote device!\n");
	for (auto it: cmds) {
		if (it->getType()==RemoteCmd::SDO_WRITE &&
				e->getIndex()==it->getEntry()->getIndex() &&
				e->getSubIndex()==it->getEntry()->getSubIndex() &&
				!it->isInitialized()) {
			//printf("have a queued already...\n");
			return;
		}
	}

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

VoidPromise RemoteDevice::flush() {
	auto c=std::make_shared<RemoteCmd>(RemoteCmd::FLUSH);
	c->setRemoteDevice(this);
	cmds.push_back(c);
	return c->getFlushPromise();
}

void RemoteDevice::handleMessage(cof_t *frame) {
	if (cmdInFlight)
		cmdInFlight->handleMessage(frame);

	if (cof_get(frame,COF_FUNC)==COF_FUNC_HEARTBEAT &&
			cof_get(frame,COF_NODE_ID)==getNodeId()) {
		//printf("got device heartbeat: %d\n",getNodeId());
		heartbeatDeadline=getBus()->millis()+3000;

		if (state!=OPERATIONAL) {
			state=OPERATIONAL;
			stateChangeEvent.emit(getState());
		}
	}

	for (auto pdo: pdos) {
		int pdoId=0x180+((pdo->getPdoNum()-1)*0x100)+getNodeId();
		if (frame->id==pdoId) {
			auto pdoEntry=at(0x1A00+pdo->getPdoNum()-1,1);
			uint8_t bits=pdoEntry->getData(0);
			uint8_t subindex=pdoEntry->getData(1);
			uint16_t index=pdoEntry->getData(2)+(pdoEntry->getData(3)<<8);

			//printf("got pdo update, %04x:%02x ...\n",index,subindex);
			auto entry=at(index,subindex);

			for (int i=0; i<4; i++) {
				suppressChangeNotification();
				entry->setData(i,frame->data[i]);
			}

			entry->changeDispatcher.emit();
		}
	}
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

	if (getBus()->millis()>heartbeatDeadline &&
			state!=DISCONNECTED) {
		//printf("heartbeat deadline expired\n");
		state=DISCONNECTED;
		stateChangeEvent.emit(getState());
	}
}

std::string RemoteDevice::getState() {
	switch (state) {
		case DISCONNECTED:
			return "disconnected";

		case OPERATIONAL:
			return "operational";

		default:
			printf("warning!! uknown state: %d\n",state);
			return "<unknown>";
	}
}

std::shared_ptr<Bus> RemoteDevice::getBus() { 
	return masterDevice->getBus(); 
}

void RemoteDevice::setMasterDevice(MasterDevice *masterDevice_) { 
	if (masterDevice)
		throw std::runtime_error("can't change master device");

	masterDevice=masterDevice_; 
	heartbeatDeadline=getBus()->millis();

	loopHandlerId=getBus()->loopDispatcher.on([this]() {
		handleLoop();
	});

	messageHandlerId=getBus()->messageDispatcher.on([this](cof_t *frame) {
		handleMessage(frame);
	});
}

