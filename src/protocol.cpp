#include "canopener/protocol.h"
#include "canopener/RemoteDevice.h"
#include <stdio.h>

using namespace canopener;

void canopener::handleSegmentedUpload(Device *dev, cof_t *frame) {
    bool filter =
        cof_get(frame, COF_FUNC) == COF_FUNC_SDO_RX &&
        cof_get(frame, COF_SDO_CMD) == COF_SDO_CMD_SEGMENT_UPLOAD &&
        cof_get(frame, COF_NODE_ID) == dev->getNodeId();

    if (!filter)
        return;

    //printf("segmented upload index: %d\n",dev->segmentedUploadIndex);

    if (!dev->segmentedUploadIndex) {
        printf("no segmented upload in progress...\n");

        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, dev->getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_SCS_ABORT);
        cof_set(&abort, COF_SDO_INDEX, 0);
        cof_set(&abort, COF_SDO_SUBINDEX, 0);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_UNSUPPORTED);
        dev->getBus()->write(&abort);
        return;
    }

    if (dev->segmentedUploadToggleBit!=cof_get(frame,COF_SDO_TOGGLE)) {
        printf("wrong segmented toggle bit...\n");

        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, dev->getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_SCS_ABORT);
        cof_set(&abort, COF_SDO_INDEX, dev->segmentedUploadIndex);
        cof_set(&abort, COF_SDO_SUBINDEX, dev->segmentedUploadSubIndex);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_CMD);
        dev->getBus()->write(&abort);
        return;

    }

    std::shared_ptr<Entry> e=dev->find(dev->segmentedUploadIndex,dev->segmentedUploadSubIndex);

    cof_t reply;
    cof_init(&reply);
    cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
    cof_set(&reply, COF_NODE_ID, dev->getNodeId());
    cof_set(&reply, COF_SDO_TOGGLE, dev->segmentedUploadToggleBit);
    cof_set(&reply, COF_SDO_COMPLETE, 0);

    reply.len=8;
    for (int i=0; i<7; i++)
        reply.data[i+1]=e->getData(i+dev->segmentedUploadOffset);

    dev->segmentedUploadOffset+=7;
    dev->getBus()->write(&reply);
}

void canopener::handleUploadRequest(Device *dev, cof_t *frame) {
    bool filter =
        cof_get(frame, COF_FUNC) == COF_FUNC_SDO_RX &&
        cof_get(frame, COF_SDO_CMD) == COF_SDO_CMD_UPLOAD &&
        cof_get(frame, COF_NODE_ID) == dev->getNodeId();

    if (!filter)
        return;

    //printf("it is...\n");

    uint16_t idx = cof_get(frame, COF_SDO_INDEX);
    uint8_t  sub = cof_get(frame, COF_SDO_SUBINDEX);

    std::shared_ptr<Entry> e = dev->find(idx, sub);
    if (!e) {
        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, dev->getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_SCS_ABORT); // server abort = 4
        cof_set(&abort, COF_SDO_INDEX, idx);
        cof_set(&abort, COF_SDO_SUBINDEX, sub);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_NOT_EXIST);
        dev->getBus()->write(&abort);
        return;
    }

    // segmented
    if (e->size()>4) {
        //printf("doing segmented upload!!!\n");
        dev->segmentedUploadIndex=idx;
        dev->segmentedUploadSubIndex=sub;
        dev->segmentedUploadToggleBit=false;
        dev->segmentedUploadOffset=0;

        //printf("set segmented upload index: %d\n",dev->segmentedUploadIndex);

        cof_t reply;
        cof_init(&reply);
        cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&reply, COF_NODE_ID, dev->getNodeId());
        cof_set(&reply, COF_SDO_CMD, COF_SDO_SCS_UPLOAD_REPLY);
        cof_set(&reply, COF_SDO_EXPEDITED, 0);
        cof_set(&reply, COF_SDO_SIZE_IND, 1);
        cof_set(&reply, COF_SDO_INDEX, idx);
        cof_set(&reply, COF_SDO_SUBINDEX, sub);
        cof_set(&reply, COF_SDO_SIZE, e->size());
        dev->getBus()->write(&reply);
    }

    // expedited
    else {
        cof_t reply;
        cof_init(&reply);
        size_t size=e->size();
        if (size>4)
            size=4;

        cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&reply, COF_NODE_ID, dev->getNodeId());
        cof_set(&reply, COF_SDO_CMD, COF_SDO_SCS_UPLOAD_REPLY);
        cof_set(&reply, COF_SDO_EXPEDITED, 1);
        cof_set(&reply, COF_SDO_SIZE_IND, 1);
        cof_set(&reply, COF_SDO_N_UNUSED, 4 - size);
        cof_set(&reply, COF_SDO_INDEX, idx);
        cof_set(&reply, COF_SDO_SUBINDEX, sub);

        for (int i=0; i<size; i++)
            cof_set(&reply, COF_SDO_DATA_0+i, e->getData(i));

        dev->getBus()->write(&reply);
    }
}

void canopener::handleSdoExpeditedWrite(Device *dev, cof_t *frame) {
    bool filter =
        cof_get(frame, COF_FUNC) == COF_FUNC_SDO_RX &&
        cof_get(frame, COF_SDO_CMD) == COF_SDO_CMD_DOWNLOAD &&
        cof_get(frame, COF_SDO_EXPEDITED) &&
        cof_get(frame, COF_NODE_ID) == dev->getNodeId();

    if (!filter)
        return;

    uint16_t idx = cof_get(frame, COF_SDO_INDEX);
    uint8_t sub  = cof_get(frame, COF_SDO_SUBINDEX);

    std::shared_ptr<Entry> e=dev->find(idx, sub);
    if (!e) {
        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, dev->getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_CMD_ABORT);
        cof_set(&abort, COF_SDO_INDEX, idx);
        cof_set(&abort, COF_SDO_SUBINDEX, sub);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_NOT_EXIST);
        dev->getBus()->write(&abort);
        return;
    }

    size_t size = 4 - cof_get(frame, COF_SDO_N_UNUSED);
    if (size > e->size()) size = e->size();

    for (int i=0; i<size; i++) {
        dev->suppressChangeNotification();
        e->setData(i,cof_get(frame,COF_SDO_DATA_0+i));
    }

    cof_t reply;
    cof_init(&reply);
    cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
    cof_set(&reply, COF_SDO_CMD, COF_SDO_SCS_DOWNLOAD_REPLY);
    cof_set(&reply, COF_NODE_ID, dev->getNodeId());
    cof_set(&reply, COF_SDO_INDEX, idx);
    cof_set(&reply, COF_SDO_SUBINDEX, sub);
    dev->getBus()->write(&reply);
}

void canopener::performSdoExpeditedWrite(RemoteDevice *dev, Entry *e) {
    //printf("doing write...\n");

    cof_t cof;
    cof_init(&cof);

    int expWriteSize=e->size();
    if (expWriteSize>4)
        expWriteSize=4;

    cof_set(&cof,COF_FUNC, COF_FUNC_SDO_RX);
    cof_set(&cof,COF_NODE_ID, dev->getNodeId());
    cof_set(&cof,COF_SDO_CMD, COF_SDO_CMD_DOWNLOAD);
    cof_set(&cof,COF_SDO_EXPEDITED, 1);
    cof_set(&cof,COF_SDO_SIZE_IND, 1);
    cof_set(&cof,COF_SDO_N_UNUSED,4 - expWriteSize);
    cof_set(&cof,COF_SDO_INDEX, e->getIndex());
    cof_set(&cof,COF_SDO_SUBINDEX, e->getSubIndex());

    for (int i=0; i<expWriteSize; i++)
        cof_set(&cof,COF_SDO_DATA_0+i,e->getData(i));

    dev->getBus()->write(&cof);
}

void canopener::performSdoExpeditedRead(RemoteDevice *dev, Entry *e) {
    cof_t cof;
    cof_init(&cof);

    cof_set(&cof,COF_FUNC,COF_FUNC_SDO_RX);
    cof_set(&cof,COF_NODE_ID,dev->getNodeId());
    cof_set(&cof,COF_SDO_CMD,COF_SDO_CMD_UPLOAD);
    cof_set(&cof,COF_SDO_INDEX,e->getIndex()); 
    cof_set(&cof,COF_SDO_SUBINDEX,e->getSubIndex());

    dev->getBus()->write(&cof);
}