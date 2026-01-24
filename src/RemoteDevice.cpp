#include "canopener.h"
#include <stdexcept>

using namespace canopener;

RemoteDevice::RemoteDevice(int nodeId_) { 
	nodeId=nodeId_; 
	/*insert(0x1A00,1);
	insert(0x1A01,1);
	insert(0x1A02,1);
	insert(0x1A03,1);*/
}

void RemoteDevice::handleMessage(cof_t *frame) {
	if (sdoReadEntry &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_UPLOAD_REPLY &&
			cof_get(frame,COF_SDO_INDEX)==sdoReadEntry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==sdoReadEntry->getSubIndex()) {
		int size=4-cof_get(frame,COF_SDO_N_UNUSED);

        for (int i=0; i<size; i++)
		    sdoReadEntry->setData(i,cof_get(frame,COF_SDO_DATA_0+i));

    	//Serial.printf("got sdo read reply %04x:%02x size=%d val=%d\n",sdoReadEntry->getIndex(),sdoReadEntry->getSubIndex(),size,sdoReadEntry->get<int32_t>());

		sdoReadEntry->clearDirty();
		sdoReadEntry=nullptr;
		commitGenerationChangeDispatcher.emit();
	}

	if (sdoWriteEntry &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_DOWNLOAD_REPLY &&
			cof_get(frame,COF_SDO_INDEX)==sdoWriteEntry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==sdoWriteEntry->getSubIndex()) {
		sdoWriteEntry->commitGeneration=sdoWriteGeneration;
		sdoWriteEntry=nullptr;
		commitGenerationChangeDispatcher.emit();
	}

	for (int pdoIndex=0; pdoIndex<4; pdoIndex++) {
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

			//entry.emit("change");*/
		}
	}
}

void RemoteDevice::handleLoop() {
	for (Entry* e: entries) {
		if (!sdoWriteEntry && e->dirty) {
			e->dirty=false;
			sdoWriteEntry=e;
			sdoWriteDeadline=getBus().millis();
		}
	}

	if (sdoWriteEntry && getBus().millis()>=sdoWriteDeadline) {
		sdoWriteEntry->dirty=false;
		sdoWriteDeadline=getBus().millis()+1000;
		sdoWriteGeneration=sdoWriteEntry->generation;
		performSdoExpeditedWrite(*this,*sdoWriteEntry);
	}

	for (Entry* e: entries) {
		if (!sdoReadEntry && e->refreshRequested) {
			e->refreshRequested=false;
			sdoReadEntry=e;
			sdoReadDeadline=getBus().millis();
		}
	}

	if (sdoReadEntry && getBus().millis()>=sdoReadDeadline) {
		sdoReadEntry->refreshRequested=false;
		sdoReadDeadline=getBus().millis()+1000;
    	//Serial.printf("sending read %04x:%02x\n",sdoReadEntry->getIndex(),sdoReadEntry->getSubIndex());

		performSdoExpeditedRead(*this,*sdoReadEntry);
	}
}

Bus& RemoteDevice::getBus() { 
	return masterDevice->getBus(); 
}

void RemoteDevice::setMasterDevice(MasterDevice *masterDevice_) { 
	if (masterDevice)
		throw std::runtime_error("can't change master device");

	masterDevice=masterDevice_; 

	getBus().loopDispatcher.on([this]() {
		handleLoop();
	});

	getBus().messageDispatcher.on([this](cof_t *frame) {
		handleMessage(frame);
	});
}

bool RemoteDevice::isRefreshInProgress() {
	if (sdoReadEntry)
		return true;

	for (Entry* e: entries)
		if (e->refreshRequested)
			return true;

	return false;
}