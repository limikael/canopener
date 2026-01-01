#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "canopener/cof-defines.h"
#include <stdint.h>

#define COF_COB_ID 3000
#define COF_DLC 3001

typedef struct {
    uint32_t id;
    uint8_t  len;
    uint8_t  data[8];
} cof_t;

void cof_init(cof_t *frame);
cof_t *cof_create();
void cof_dispose(cof_t *cof);
void cof_set(cof_t *frame, int prop, uint32_t value);
uint32_t cof_get(const cof_t *frame, int prop);
char *cof_to_slcan(cof_t *f, char *s);
cof_t *cof_from_slcan(cof_t *f, const char *buf);
void cof_cpy(cof_t *dest, cof_t *src);

#ifdef __cplusplus
}
#endif
