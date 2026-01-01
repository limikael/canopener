#include "canopener/cof.h"
#include "canopener/cof-defines.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t get_bits(const uint8_t *buf, uint32_t bit, uint32_t len)
{
    uint32_t val = 0;

    for (uint32_t i = 0; i < len; i++) {
        uint32_t b = bit + i;
        uint32_t byte = b >> 3;
        uint32_t bit_in_byte = b & 7;

        if (buf[byte] & (1u << bit_in_byte))
            val |= (1u << i);
    }
    return val;
}

static void set_bits(uint8_t *buf, uint32_t bit, uint32_t len, uint32_t val)
{
    for (uint32_t i = 0; i < len; i++) {
        uint32_t b = bit + i;
        uint32_t byte = b >> 3;
        uint32_t bit_in_byte = b & 7;

        uint8_t mask = (1u << bit_in_byte);

        if (val & (1u << i))
            buf[byte] |= mask;
        else
            buf[byte] &= ~mask;
    }
}

static void u32_to_le(uint32_t v, uint8_t out[4])
{
    out[0] = (uint8_t)(v >> 0);
    out[1] = (uint8_t)(v >> 8);
    out[2] = (uint8_t)(v >> 16);
    out[3] = (uint8_t)(v >> 24);
}

static uint32_t le_to_u32(const uint8_t in[4])
{
    return ((uint32_t)in[0] << 0)  |
           ((uint32_t)in[1] << 8)  |
           ((uint32_t)in[2] << 16) |
           ((uint32_t)in[3] << 24);
}

uint32_t cof_get(const cof_t *cof, int prop) {
    if (prop>=2000 && prop<3000) {
        const cof_field_t *f=&cof_cob_bits[prop-2000];
        uint8_t buf[4];
        u32_to_le(cof->id, buf);
        return get_bits(buf, f->offs, f->len);
    }

    if (prop>=1000 && prop<2000) {
        const cof_field_t *f=&cof_data_bits[prop-1000];
        return get_bits(cof->data, f->offs, f->len);
    }

    switch (prop) {
        case COF_COB_ID: return cof->id;
        case COF_DLC: return cof->len;
    }

    printf("read unknown prop: %d\n",prop);
    assert(false);
}

void cof_set(cof_t *cof, int prop, uint32_t value) {
    if (prop>=2000 && prop<3000) {
        const cof_field_t *f=&cof_cob_bits[prop-2000];
        uint8_t buf[4];
        u32_to_le(cof->id, buf);
        set_bits(buf, f->offs, f->len, value);
        cof->id=le_to_u32(buf)&0x7FF;  // clamp to 11 bits 
        return;
    }

    if (prop>=1000 && prop<2000) {
        const cof_field_t *f=&cof_data_bits[prop-1000];
        if (f->len < 32)
            value &= (1u << f->len) - 1;

        set_bits(cof->data, f->offs, f->len, value);
        uint8_t max_byte = (f->offs + f->len + 7) >> 3;
        if (cof->len < max_byte)
            cof->len = max_byte;

        return;
    }

    switch (prop) {
        case COF_COB_ID: cof->id=value; return;
        case COF_DLC: cof->len=value; return;
    }

    printf("write unknown prop: %d\n",prop);
    assert(false);
}

void cof_init(cof_t *frame) {
    frame->id = 0;
    frame->len = 0;
    memset(frame->data, 0, 8);//COF_MAX_DATA);
}

cof_t *cof_create() {
    cof_t *cof=(cof_t *)malloc(sizeof(cof_t));
    cof_init(cof);
    return cof;
}

void cof_dispose(cof_t *cof) {
    if (!cof)
        return;

    free(cof);
}

char *cof_to_slcan(cof_t *f, char *buf) {
    uint8_t pos = 0;

    if (cof_get(f,COF_COB_ID) > 0x7FF) {
        buf[pos++] = 'T'; // extended
        sprintf(buf + pos, "%08X", cof_get(f,COF_COB_ID));
        pos += 8;
    } else {
        buf[pos++] = 't'; // standard
        sprintf(buf + pos, "%03X", cof_get(f,COF_COB_ID));
        pos += 3;
    }

    //printf("size: %d\n",cof_get(f,COF_DLC));
    buf[pos++] = '0' + cof_get(f,COF_DLC);
    //printf("size: %d\n",cof_get(f,COF_DLC));

    for (uint8_t i = 0; i < cof_get(f,COF_DLC); i++) {
        //printf("size: %d, i=%d\n",cof_get(f,COF_DLC),i);
        sprintf(buf+pos, "%02X", f->data[i]);//cof_getp(f,COF_DATA)[i]);
        pos += 2;
    }

    buf[pos] = 0;

    return buf;
}

cof_t *cof_from_slcan(cof_t *f, const char *buf) {
    //printf("here... buf=%d\n",buf);
    //printf("buf[0]=%c\n",buf[0]);
    bool ext = false;

    if (buf[0] == 't') ext = false;
    else if (buf[0] == 'T') ext = true;
    else { return NULL; }

    uint8_t pos = 1;

    // Parse ID
    uint8_t idChars = ext ? 8 : 3;
    char ids[8];
    memcpy(ids,buf+pos,idChars);
    ids[idChars]='\n';

    f->id = strtoul(ids, NULL, 16);
    pos += idChars;

    // DLC
    f->len = buf[pos++] - '0';
    if (f->len > 8) f->len = 8;

    // Data bytes
    for (uint8_t i = 0; i < f->len; i++) {
        char byteStr[3] = { buf[pos], buf[pos+1], 0 };
        f->data[i] = strtoul(byteStr, NULL, 16);
        pos += 2;
    }

    return f;
}

void cof_cpy(cof_t *dest, cof_t *src) {
    memcpy(dest,src,sizeof(cof_t));
}
