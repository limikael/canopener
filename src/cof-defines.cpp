#include "canopener/cof-defines.h"

cof_field_t cof_data_bits[]={
    {.offs=5, .len=3}, // COF_SDO_CMD,
    {.offs=2, .len=2}, // COF_SDO_N_UNUSED,
    {.offs=1, .len=1}, // COF_SDO_EXPEDITED,
    {.offs=0, .len=1}, // COF_SDO_SIZE_IND,
    {.offs=8, .len=16}, // COF_SDO_INDEX,
    {.offs=24, .len=8}, // COF_SDO_SUBINDEX,
    {.offs=32, .len=8}, // COF_SDO_DATA_0,
    {.offs=40, .len=8}, // COF_SDO_DATA_1,
    {.offs=48, .len=8}, // COF_SDO_DATA_2,
    {.offs=56, .len=8}, // COF_SDO_DATA_3,
    {.offs=32, .len=32}, // COF_SDO_ABORT_CODE,
    {.offs=0, .len=8}, // COF_HEARTBEAT_STATE,
};

cof_field_t cof_cob_bits[]={
    {.offs=7, .len=4}, // COF_FUNC,
    {.offs=0, .len=7}, // COF_NODE_ID,
};
