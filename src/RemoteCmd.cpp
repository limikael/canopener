#include "canopener/RemoteCmd.h"
#include <cstdio>

using namespace canopener;

RemoteCmd::RemoteCmd(Type t, std::shared_ptr<Entry> e) {
	type=t;
	entry=e;
}

void RemoteCmd::handleLoop() {
	if (complete)
		return;

	uint32_t now=remoteDevice->getBus()->millis();

	if (!initialized) {
		//printf("init cmd!\n");
		initialized=true;
		deadline=now;
	}

	switch (type) {
		case Type::SDO_WRITE:
			if (now>=deadline) {
				deadline=now+1000;
				performSdoExpeditedWrite(remoteDevice,entry.get());
			}
			break;

		case Type::SDO_READ:
			if (now>=deadline) {
				deadline=now+1000;
				performSdoExpeditedRead(remoteDevice,entry.get());
			}
			break;
	}
}

void RemoteCmd::handleMessage(cof_t *frame) {
	if (type==Type::SDO_WRITE &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_DOWNLOAD_REPLY &&
			cof_get(frame,COF_SDO_INDEX)==entry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==entry->getSubIndex()) {
		//printf("got write response\n");
		complete=true;
	}

	if (type==Type::SDO_READ &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_UPLOAD_REPLY &&
			cof_get(frame,COF_SDO_INDEX)==entry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==entry->getSubIndex()) {
		int size=4-cof_get(frame,COF_SDO_N_UNUSED);

        for (int i=0; i<size; i++) {
	        remoteDevice->suppressChangeNotification();
		    entry->setData(i,cof_get(frame,COF_SDO_DATA_0+i));
        }

    	//Serial.printf("got sdo read reply %04x:%02x size=%d val=%d\n",sdoReadEntry->getIndex(),sdoReadEntry->getSubIndex(),size,sdoReadEntry->get<int32_t>());
		complete=true;
	}
}

bool RemoteCmd::isComplete() {
	return complete;
}