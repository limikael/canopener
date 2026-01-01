#include "canopener.h"
#include <stdio.h>

using namespace canopener;

void canopener::handleSdoExpeditedRead(Device &dev, cof_t *frame) {
    //printf("here, type=%02x nodeid=%d\n",cof_get(frame,COF_TYPE),cof_get(frame,COF_NODE_ID)); fflush(stdout);

    /*if (cof_get(frame,COF_TYPE)!=COF_TYPE_SDO_UPLOAD_REQUEST ||
            cof_get(frame,COF_NODE_ID)!=dev.getNodeId())
        return;

    uint16_t idx=cof_get(frame,COF_SDO_INDEX);
    uint8_t sub=cof_get(frame,COF_SDO_SUB);
    Entry *e=dev.find(idx, sub);
    if (!e) {
        //printf("will abort!!!\n");
        cof_t abort;
        cof_set(&abort,COF_TYPE,COF_TYPE_SDO_ABORT); 
        cof_set(&abort,COF_NODE_ID,dev.getNodeId()); 
        cof_set(&abort,COF_SDO_INDEX,idx); 
        cof_set(&abort,COF_SDO_SUB,sub);
        cof_set(&abort,COF_SDO_ABORT_CODE,COF_ABORT_NOT_EXIST);
        dev.getBus().write(&abort);
        return;
    }

    const auto &bytes = e->raw();
    size_t size = bytes.size();
    if (size>4)
        size=4;

    cof_t reply;
    cof_set(&reply,COF_TYPE,COF_TYPE_SDO_UPLOAD_REPLY); 
    cof_set(&reply,COF_NODE_ID,dev.getNodeId()); 
    cof_set(&reply,COF_SDO_INDEX,idx); 
    cof_set(&reply,COF_SDO_SUB,sub);
    cof_set(&reply,COF_SDO_EXPEDITED,1);
    cof_set(&reply,COF_SDO_SIZE_IND,1);
    cof_set(&reply,COF_SDO_EXP_SIZE,size);
    memcpy(cof_getp(&reply,COF_SDO_EXP_DATA),bytes.data(),size);
    dev.getBus().write(&reply);*/

    //printf("it is an upload request!!!\n"); fflush(stdout);
}

/*void canopener::handleSdoExpeditedWrite(Device &dev, cof_t *frame) {
    //printf("here, type=%02x\n",cof_get(frame,COF_TYPE));

    if (cof_get(frame,COF_TYPE)!=COF_TYPE_SDO_DOWNLOAD_REQUEST ||
            !cof_get(frame,COF_SDO_EXPEDITED) ||
            cof_get(frame,COF_NODE_ID)!=dev.getNodeId())
        return;

    //printf("yes!!!\n"); fflush(stdout);
    uint16_t idx=cof_get(frame,COF_SDO_INDEX);
    uint8_t sub=cof_get(frame,COF_SDO_SUB);
    Entry *e=dev.find(idx, sub);
    if (!e) {
        //printf("will abort!!!\n");
        cof_t abort;
        cof_set(&abort,COF_TYPE,COF_TYPE_SDO_ABORT); 
        cof_set(&abort,COF_NODE_ID,dev.getNodeId()); 
        cof_set(&abort,COF_SDO_INDEX,idx); 
        cof_set(&abort,COF_SDO_SUB,sub);
        cof_set(&abort,COF_SDO_ABORT_CODE,COF_ABORT_NOT_EXIST);
        dev.getBus().write(&abort);
        return;
    }

    //printf("setting, expedited size: %d\n",cof_get(frame,COF_SDO_EXP_SIZE));

    size_t size=cof_get(frame,COF_SDO_EXP_SIZE);
    if (size>e->raw().size())
        size=e->raw().size();

    e->setRaw(cof_getp(frame,COF_SDO_EXP_DATA),size);

    cof_t reply;
    cof_set(&reply,COF_TYPE,COF_TYPE_SDO_DOWNLOAD_REPLY); 
    cof_set(&reply,COF_NODE_ID,dev.getNodeId()); 
    cof_set(&reply,COF_SDO_INDEX,idx); 
    cof_set(&reply,COF_SDO_SUB,sub);
    dev.getBus().write(&reply);
}*/

void canopener::handleSdoExpeditedWrite(Device &dev, cof_t *frame) {
    // Check filter first
    bool filter =
        cof_get(frame, COF_FUNC) == COF_FUNC_SDO_RX &&
        cof_get(frame, COF_SDO_CMD) == COF_SDO_CMD_DOWNLOAD &&
        cof_get(frame, COF_SDO_EXPEDITED) &&
        cof_get(frame, COF_NODE_ID) == dev.getNodeId();

    if (!filter)
        return;

    uint16_t idx = cof_get(frame, COF_SDO_INDEX);
    uint8_t sub  = cof_get(frame, COF_SDO_SUBINDEX);

    Entry *e = dev.find(idx, sub);
    if (!e) {
        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, dev.getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_CMD_ABORT);
        cof_set(&abort, COF_SDO_INDEX, idx);
        cof_set(&abort, COF_SDO_SUBINDEX, sub);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_NOT_EXIST);
        dev.getBus().write(&abort);
        return;
    }

    size_t size = 4 - cof_get(frame, COF_SDO_N_UNUSED);
    if (size > e->size()) size = e->size();

    for (int i=0; i<size; i++)
        e->setData(i,cof_get(frame,COF_SDO_DATA_0+i));

    cof_t reply;
    cof_init(&reply);
    cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
    cof_set(&reply, COF_SDO_CMD, COF_SDO_SCS_DOWNLOAD_REPLY);
    cof_set(&reply, COF_NODE_ID, dev.getNodeId());
    cof_set(&reply, COF_SDO_INDEX, idx);
    cof_set(&reply, COF_SDO_SUBINDEX, sub);
    dev.getBus().write(&reply);
}
