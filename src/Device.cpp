#include "canopener/Device.h"
#include "canopener/DeviceSegmentedOp.h"
#include "canopener/Pdo.h"
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

	segmentedOp=nullptr;
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

	if (!segmentedOp &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_RX &&
			cof_get(frame,COF_NODE_ID)==getNodeId() &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_CMD_SEGMENT_UPLOAD) {
		//printf("segment request, but no segmented upload in progress...\n");
		cof_t abort;
        cof_init(&abort);
        cof_set(&abort,COF_FUNC,COF_FUNC_SDO_TX);
        cof_set(&abort,COF_NODE_ID,getNodeId());
        cof_set(&abort,COF_SDO_CMD,COF_SDO_SCS_ABORT);
        cof_set(&abort,COF_SDO_INDEX,0);
        cof_set(&abort,COF_SDO_SUBINDEX,0);
        cof_set(&abort,COF_SDO_ABORT_CODE,COF_ABORT_CMD);
        getBus()->write(&abort);
	}

	if (segmentedOp)
		segmentedOp->handleMessage(frame);

	handleUploadRequest(this,frame);
	handleSdoExpeditedWrite(this,frame);

	if (cof_get(frame,COF_FUNC)==COF_FUNC_HEARTBEAT &&
			cof_get(frame,COF_NODE_ID)==1) {
		//Serial.printf("master heartbeat...\n");
		masterHeartbeatDeadline=bus->millis()+3000;
		state=OPERATIONAL;
	}
}

void Device::handleLoop() {
	if (segmentedOp)
		segmentedOp->handleLoop();

	if (segmentedOp && segmentedOp->isComplete())
		segmentedOp=nullptr;

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

	for (auto pdo: pdos) {
		if (pdo->dirtyOutgoing) {
			//printf("dirty outgoing, index=%d, subIndex=%d\n",pdo->getMappedIndex(),pdo->getMappedSubIndex());
			pdo->dirtyOutgoing=false;
			auto e=at(pdo->getMappedIndex(),pdo->getMappedSubIndex());
			cof_t cof;
 	       	cof_init(&cof);
			cof_set(&cof,COF_COB_ID,0x180+(0x100*(pdo->pdoNum-1))+getNodeId());
			cof_set(&cof,COF_DLC,4);
			cof.data[0]=e->getData(0);
			cof.data[1]=e->getData(1);
			cof.data[2]=e->getData(2);
			cof.data[3]=e->getData(3);
	        getBus()->write(&cof);
		}
	}
}

void Device::handleChange(std::shared_ptr<Entry> e) {
	//printf("change in Device\n");
	for (auto pdo: pdos) {
		if (e->getIndex()==pdo->getMappedIndex() && 
				e->getSubIndex()==pdo->getMappedSubIndex()) {
			//printf("setting dirty, index=%d\n",e->getIndex());
			pdo->dirtyOutgoing=true;
		}
	}
}
