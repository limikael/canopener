#include "canopener/Device.h"
#include <stdexcept>

using namespace canopener;

Device::Device(std::shared_ptr<Bus> bus_) {
	bus=bus_;
	nodeId=0;
	heartbeatInterval=1000;
	heartbeatDeadline=bus->millis()+heartbeatInterval;
	masterHeartbeatDeadline=0;
	state=DISCONNECTED;

	handleLoopId=bus->loopDispatcher.on([this]() { handleLoop(); });
	handleMessageId=bus->messageDispatcher.on([this](cof_t *frame) { handleMessage(frame); });
	handleLoopTimeoutId=bus->loopTimeoutDispatcher.on([this](std::shared_ptr<LoopTimeoutEvent> ev) { handleLoopTimeout(ev); });

}

Device::~Device() {
	bus->loopDispatcher.off(handleLoopId);
	bus->messageDispatcher.off(handleMessageId);
	bus->loopTimeoutDispatcher.off(handleLoopTimeoutId);
}

void Device::handleLoopTimeout(std::shared_ptr<LoopTimeoutEvent> ev) {
	ev->setTimeout(heartbeatDeadline-bus->millis());
}

void Device::handleMessage(cof_t *frame) {
	//Serial.printf("handle message in device: %d m: %d from: %d\n",getNodeId(),bus.millis(),cof_get(frame,COF_NODE_ID));
	//printf("got message\n");

	handleSdoExpeditedRead(this,frame);
	handleSdoExpeditedWrite(this,frame);

	if (cof_get(frame,COF_FUNC)==COF_FUNC_HEARTBEAT &&
			cof_get(frame,COF_NODE_ID)==1) {
		//Serial.printf("master heartbeat...\n");
		masterHeartbeatDeadline=bus->millis()+3000;
		state=OPERATIONAL;
	}
}

void Device::handleLoop() {
	if (bus->millis()>=heartbeatDeadline) {

        cof_t heartbeat;
        cof_init(&heartbeat);
        cof_set(&heartbeat,COF_FUNC,COF_FUNC_HEARTBEAT); 
        cof_set(&heartbeat,COF_NODE_ID,getNodeId()); 
        cof_set(&heartbeat,COF_HEARTBEAT_STATE,COF_HB_OPERATIONAL);

        //delay(100);
		//Serial.printf("sending heartbeat for node: %d len=%d\n",getNodeId(),heartbeat.len);

        getBus()->write(&heartbeat);

		heartbeatDeadline=bus->millis()+heartbeatInterval;
	}

	if (bus->millis()>=masterHeartbeatDeadline) {
		state=DISCONNECTED;
	}
}

void Device::handleChange(std::shared_ptr<Entry> e) {
	//printf("change in Device\n");
	for (int pdoIndex=0; pdoIndex<4; pdoIndex++) {
		auto pdo=at(0x1A00+pdoIndex,1);

		uint32_t bits=pdo->getData(0);
		uint32_t subIndex=pdo->getData(1);
		uint32_t index=pdo->getData(2)+(pdo->getData(3)<<8);

		if (e->getIndex()==index && e->getSubIndex()==subIndex) {
			//printf("send pdo %x %x\n",index,subIndex);
			cof_t cof;
 	       	cof_init(&cof);
			cof_set(&cof,COF_COB_ID,0x180+(pdoIndex*0x100)+getNodeId());
			cof_set(&cof,COF_DLC,4);
			cof.data[0]=e->getData(0);
			cof.data[1]=e->getData(1);
			cof.data[2]=e->getData(2);
			cof.data[3]=e->getData(3);
	        getBus()->write(&cof);
		}
	}
}
