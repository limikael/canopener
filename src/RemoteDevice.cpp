#include "canopener.h"
#include <stdexcept>

using namespace canopener;

void RemoteDevice::handleMessage(cof_t *frame) {
	if (sdoWriteEntry &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_DOWNLOAD_REPLY &&
			cof_get(frame,COF_SDO_INDEX)==sdoWriteEntry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==sdoWriteEntry->getSubIndex()) {
		sdoWriteEntry->commitGeneration=sdoWriteGeneration;
		sdoWriteEntry=nullptr;
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
