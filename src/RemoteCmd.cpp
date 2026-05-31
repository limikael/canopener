#include "canopener/RemoteCmd.h"
#include "canopener/RemoteDevice.h"
#include <cstdio>
#include <cassert>

using namespace canopener;

RemoteCmd::RemoteCmd(Type t, std::shared_ptr<Entry> e) {
	type=t;
	entry=e;
}

RemoteCmd::RemoteCmd(Type t) {
	type=t;
	entry=nullptr;
	assert(type==Type::FLUSH);
	//flushPromise=std::make_shared<FlushPromise>();
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

		case Type::FLUSH:
			//flushPromise->flushEvent.emit();
			flushPromise.resolve();
			complete=true;
			break;
	}
}

void RemoteCmd::handleMessage(cof_t *frame) {
	if (cof_get(frame,COF_NODE_ID)!=remoteDevice->getNodeId())
		return;

	if (type==Type::SDO_WRITE &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_DOWNLOAD_REPLY &&
			cof_get(frame,COF_SDO_INDEX)==entry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==entry->getSubIndex()) {
		//printf("got write response\n");
		complete=true;
	}

	// expedited
	if (type==Type::SDO_READ &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_UPLOAD_REPLY &&
			cof_get(frame,COF_SDO_EXPEDITED) &&
			cof_get(frame,COF_SDO_INDEX)==entry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==entry->getSubIndex() &&
			!segmentedUpload) {
		int size=4-cof_get(frame,COF_SDO_N_UNUSED);

        for (int i=0; i<size; i++) {
	        remoteDevice->suppressChangeNotification();
		    entry->setData(i,cof_get(frame,COF_SDO_DATA_0+i));
        }

    	//Serial.printf("got sdo read reply %04x:%02x size=%d val=%d\n",sdoReadEntry->getIndex(),sdoReadEntry->getSubIndex(),size,sdoReadEntry->get<int32_t>());
		complete=true;
	}

	if (type==Type::SDO_READ &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			segmentedUpload) {
		//printf("got segmented chunk, offs=%d, idx=%04x, sub=%02x\n",segmentedUploadOffset,entry->getIndex(),entry->getSubIndex());

		if (cof_get(frame,COF_SDO_TOGGLE)!=segmentedToggleBit) {
			printf("warning! wrong toggle bit n response\n");
		}

		if (!segmentedUploadOffset && entry->getType()==Entry::STRING) {
			remoteDevice->suppressChangeNotification();
			entry->setString("");
		}

		for (int i=0; i<frame->len-1; i++) {
			remoteDevice->suppressChangeNotification();
			entry->setData(segmentedUploadOffset+i,frame->data[i+1]);
		}

		if (cof_get(frame,COF_SDO_COMPLETE)) {
			//printf("done!\n");
			complete=true;
		}

		else {
			//printf("requesting more...\n");
			segmentedUploadOffset+=frame->len-1;
			segmentedToggleBit=!segmentedToggleBit;
			cof_t cof;
			cof_init(&cof);
			cof_set(&cof, COF_FUNC, COF_FUNC_SDO_RX);
			cof_set(&cof, COF_NODE_ID, remoteDevice->getNodeId());
			cof_set(&cof, COF_SDO_CMD, COF_SDO_CMD_SEGMENT_UPLOAD);
			cof_set(&cof, COF_SDO_TOGGLE, segmentedToggleBit);
			remoteDevice->getBus()->write(&cof);
		}
	}

	// segmented response
	if (type==Type::SDO_READ &&
			cof_get(frame,COF_FUNC)==COF_FUNC_SDO_TX &&
			cof_get(frame,COF_SDO_CMD)==COF_SDO_SCS_UPLOAD_REPLY &&
			!cof_get(frame,COF_SDO_EXPEDITED) &&
			cof_get(frame,COF_SDO_INDEX)==entry->getIndex() &&
			cof_get(frame,COF_SDO_SUBINDEX)==entry->getSubIndex() &&
			!segmentedUpload) {
		//printf("got segmented reply...\n");
		segmentedUploadOffset=0;
		segmentedToggleBit=false;
		segmentedUpload=true;

		cof_t cof;
		cof_init(&cof);
		cof_set(&cof, COF_FUNC, COF_FUNC_SDO_RX);
		cof_set(&cof, COF_NODE_ID, remoteDevice->getNodeId());
		cof_set(&cof, COF_SDO_CMD, COF_SDO_CMD_SEGMENT_UPLOAD);
		cof_set(&cof, COF_SDO_TOGGLE, segmentedToggleBit);
		remoteDevice->getBus()->write(&cof);
	}
}

bool RemoteCmd::isComplete() {
	return complete;
}