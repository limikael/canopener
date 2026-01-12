#include "canopener.h"
#include <stdexcept>

using namespace canopener;

Entry& RemoteDevice::insert(uint16_t index, uint8_t subindex) {
	Entry* e=new Entry(index,subindex);
	entries.push_back(e);
	return *e;
}

Entry& RemoteDevice::insert(uint16_t index) {
	return insert(index,0);
}

Entry& RemoteDevice::at(uint16_t index, uint8_t subindex) {
	Entry *e=find(index,subindex);
	if (!e)
		throw std::out_of_range("Entry not found");

	return *e;
}

Entry& RemoteDevice::at(uint16_t index) {
	return at(index,0);
}

Entry* RemoteDevice::find(uint16_t index, uint8_t subindex) {
	for (Entry* e: entries)
		if (e->index==index && subindex==e->subindex)
			return e;

	return NULL;
}

void RemoteDevice::loop() {
	while (getBus().available()) {
		cof_t frame;
		getBus().read(&frame);

		if (sdoWriteEntry &&
				cof_get(&frame,COF_FUNC)==COF_FUNC_SDO_TX &&
				cof_get(&frame,COF_SDO_CMD)==COF_SDO_SCS_DOWNLOAD_REPLY &&
				cof_get(&frame,COF_SDO_INDEX)==sdoWriteEntry->getIndex() &&
				cof_get(&frame,COF_SDO_SUBINDEX)==sdoWriteEntry->getSubIndex()) {
			sdoWriteEntry=nullptr;
		}
	}

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
		performSdoExpeditedWrite(*this,*sdoWriteEntry);
	}
}

Bus& RemoteDevice::getBus() { 
	return masterDevice->getBus(); 
}