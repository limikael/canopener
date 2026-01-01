#include "canopener.h"
#include <stdio.h>

using namespace canopener;

void canopener::handleSdoExpeditedRead(Device &dev, cof_t *frame) {
    bool filter =
        cof_get(frame, COF_FUNC) == COF_FUNC_SDO_RX &&
        cof_get(frame, COF_SDO_CMD) == COF_SDO_CMD_UPLOAD && // CCS=2 (client request)
        cof_get(frame, COF_NODE_ID) == dev.getNodeId();

    if (!filter)
        return;

    uint16_t idx = cof_get(frame, COF_SDO_INDEX);
    uint8_t  sub = cof_get(frame, COF_SDO_SUBINDEX);

    Entry *e = dev.find(idx, sub);
    if (!e) {
        cof_t abort;
        cof_init(&abort);
        cof_set(&abort, COF_FUNC, COF_FUNC_SDO_TX);
        cof_set(&abort, COF_NODE_ID, dev.getNodeId());
        cof_set(&abort, COF_SDO_CMD, COF_SDO_SCS_ABORT); // server abort = 4
        cof_set(&abort, COF_SDO_INDEX, idx);
        cof_set(&abort, COF_SDO_SUBINDEX, sub);
        cof_set(&abort, COF_SDO_ABORT_CODE, COF_ABORT_NOT_EXIST);
        dev.getBus().write(&abort);
        return;
    }

    cof_t reply;
    cof_init(&reply);
    size_t size=e->size();
    if (size>4)
        size=4;

    cof_set(&reply, COF_FUNC, COF_FUNC_SDO_TX);
    cof_set(&reply, COF_NODE_ID, dev.getNodeId());
    cof_set(&reply, COF_SDO_CMD, COF_SDO_SCS_UPLOAD_REPLY); // 2 for upload reply
    cof_set(&reply, COF_SDO_EXPEDITED, 1);
    cof_set(&reply, COF_SDO_SIZE_IND, 1);
    cof_set(&reply, COF_SDO_N_UNUSED, 4 - size);
    cof_set(&reply, COF_SDO_INDEX, idx);
    cof_set(&reply, COF_SDO_SUBINDEX, sub);

    for (int i=0; i<size; i++)
        cof_set(&reply, COF_SDO_DATA_0+i, e->getData(i));

    dev.getBus().write(&reply);
}

void canopener::handleSdoExpeditedWrite(Device &dev, cof_t *frame) {
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
