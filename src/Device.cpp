#include "canopener.h"
#include <stdexcept>

using namespace canopener;

Device::Device(Bus& bus)
		:bus(bus) {
	nodeId=0;
	heartbeatInterval=1000;
	heartbeatDeadline=bus.millis()+heartbeatInterval;
	masterHeartbeatDeadline=0;
	state=DISCONNECTED;

	insert(0x1A00,1);
	insert(0x1A01,1);
	insert(0x1A02,1);
	insert(0x1A03,1);
}

Entry& Device::insert(uint16_t index, uint8_t subindex) {
	Entry* e=new Entry(index,subindex);
	entries.push_back(e);
	return *e;
}

Entry& Device::insert(uint16_t index) {
	return insert(index,0);
}

Entry& Device::at(uint16_t index, uint8_t subindex) {
	Entry *e=find(index,subindex);
	if (!e)
		throw std::out_of_range("Entry not found");

	return *e;
}

Entry& Device::at(uint16_t index) {
	return at(index,0);
}

Entry* Device::find(uint16_t index, uint8_t subindex) {
	for (Entry* e: entries)
		if (e->index==index && subindex==e->subindex)
			return e;

	return NULL;
}

void Device::loop() {
	bus.loop();

	while (bus.available()) {
		cof_t frame;
		bus.read(&frame);
		handleSdoExpeditedRead(*this,&frame);
		handleSdoExpeditedWrite(*this,&frame);

		if (cof_get(&frame,COF_FUNC)==COF_FUNC_HEARTBEAT &&
				cof_get(&frame,COF_NODE_ID)==1) {
			//Serial.printf("master heartbeat...\n");
			masterHeartbeatDeadline=bus.millis()+3000;
			state=OPERATIONAL;
		}
	}

	if (bus.millis()>=heartbeatDeadline) {
        cof_t heartbeat;
        cof_set(&heartbeat,COF_FUNC,COF_FUNC_HEARTBEAT); 
        cof_set(&heartbeat,COF_NODE_ID,getNodeId()); 
        cof_set(&heartbeat,COF_HEARTBEAT_STATE,COF_HB_OPERATIONAL);
        getBus().write(&heartbeat);

        //char s[256];
        //cof_to_slcan(&heartbeat,s);
		//Serial.printf("heartbeat...\n");

		heartbeatDeadline=bus.millis()+heartbeatInterval;
	}

	if (bus.millis()>=masterHeartbeatDeadline) {
		state=DISCONNECTED;
	}

	for (int pdoIndex=0; pdoIndex<4; pdoIndex++) {
		Entry& pdo=at(0x1A00+pdoIndex,1);

		uint32_t bits=pdo.getData(0);
		uint32_t subIndex=pdo.getData(1);
		uint32_t index=pdo.getData(2)+(pdo.getData(3)<<8);

		if (find(index,subIndex)) {
			Entry& e=at(index,subIndex);
			if (e.dirty) {
				//Serial.printf("send pdo %x %x\n",index,subIndex);

				cof_t cof;
				cof_set(&cof,COF_COB_ID,0x180+(pdoIndex*0x100)+getNodeId());
				cof_set(&cof,COF_DLC,4);
				cof.data[0]=e.getData(0);
				cof.data[1]=e.getData(1);
				cof.data[2]=e.getData(2);
				cof.data[3]=e.getData(3);
		        getBus().write(&cof);
				e.dirty=false;
			}
		}
	}
}
