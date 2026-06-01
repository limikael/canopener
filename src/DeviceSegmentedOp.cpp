#include "canopener/DeviceSegmentedOp.h"
#include "canopener/Device.h"
#include "canopener/cof.h"
#include <cassert>

using namespace canopener;

std::unique_ptr<DeviceSegmentedOp> DeviceSegmentedOp::createUpload(Device *dev_, int idx_, int sub_) {
	auto op=std::unique_ptr<DeviceSegmentedOp>(new DeviceSegmentedOp());

	op->device=dev_;
	op->index=idx_;
	op->subIndex=sub_;

	return op;
}

void DeviceSegmentedOp::sendUploadResponse() {
    cof_t reply;
    cof_init(&reply);
    cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
    cof_set(&reply, COF_NODE_ID, device->getNodeId());
    cof_set(&reply, COF_SDO_CMD, COF_SDO_SCS_UPLOAD_REPLY);
    cof_set(&reply, COF_SDO_EXPEDITED, 0);
    cof_set(&reply, COF_SDO_SIZE_IND, 1);
    cof_set(&reply, COF_SDO_INDEX, index);
    cof_set(&reply, COF_SDO_SUBINDEX, subIndex);
    cof_set(&reply, COF_SDO_SIZE, device->at(index,subIndex)->size());
    device->getBus()->write(&reply);
}

void DeviceSegmentedOp::handleMessage(cof_t *frame) {
	if (complete)
		return;

    bool filter =
        cof_get(frame, COF_FUNC) == COF_FUNC_SDO_RX &&
        cof_get(frame, COF_SDO_CMD) == COF_SDO_CMD_SEGMENT_UPLOAD &&
        cof_get(frame, COF_NODE_ID) == device->getNodeId();

    if (!filter)
        return;

    //printf("segmented upload index: %d\n",dev->segmentedUploadIndex);

    if (!index) {
        printf("no segmented upload in progress...\n");

        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, device->getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_SCS_ABORT);
        cof_set(&abort, COF_SDO_INDEX, 0);
        cof_set(&abort, COF_SDO_SUBINDEX, 0);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_UNSUPPORTED);
        device->getBus()->write(&abort);
        return;
    }

    if (toggleBit!=cof_get(frame,COF_SDO_TOGGLE)) {
        printf("wrong segmented toggle bit...\n");

        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, device->getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_SCS_ABORT);
        cof_set(&abort, COF_SDO_INDEX, index);
        cof_set(&abort, COF_SDO_SUBINDEX, subIndex);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_CMD);
        device->getBus()->write(&abort);
        return;
    }

    std::shared_ptr<Entry> e=device->find(index,subIndex);

    cof_t reply;
    cof_init(&reply);
    cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
    cof_set(&reply, COF_NODE_ID, device->getNodeId());
    cof_set(&reply, COF_SDO_TOGGLE, toggleBit);
    cof_set(&reply, COF_SDO_COMPLETE, 0);

    if (offset+7>=e->size())
        cof_set(&reply, COF_SDO_COMPLETE, 1);

    int chunkSize=e->size()-offset;
    if (chunkSize>7)
        chunkSize=7;

    reply.len=chunkSize+1;
    for (int i=0; i<chunkSize; i++)
        reply.data[i+1]=e->getData(i+offset);

    toggleBit=!toggleBit;
    offset+=chunkSize;
    if (offset>=e->size())
    	complete=true;

    device->getBus()->write(&reply);
}
