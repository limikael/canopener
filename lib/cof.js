import cofSchema from "./cof-schema.js";
import {slcanStringify, slcanParse} from "./slcan.js";

function getBits(buf, bit, len) {
    let val = 0;

    for (let i = 0; i < len; i++) {
        const b = bit + i;
        const byte = b >> 3;
        const bitInByte = b & 7;

        if (buf[byte] & (1 << bitInByte)) {
            val |= (1 << i);
        }
    }

    // force uint32 semantics
    return val >>> 0;
}

function setBits(buf, bit, len, val) {
    for (let i = 0; i < len; i++) {
        const b = bit + i;
        const byte = b >> 3;
        const bitInByte = b & 7;

        const mask = 1 << bitInByte;

        if (val & (1 << i)) {
            buf[byte] |= mask;
        } else {
            buf[byte] &= ~mask;
        }
    }
}

function u32ToLe(v, out) {
    out[0] = (v >>> 0) & 0xFF;
    out[1] = (v >>> 8) & 0xFF;
    out[2] = (v >>> 16) & 0xFF;
    out[3] = (v >>> 24) & 0xFF;
}

function leToU32(buf) {
    return (
        (buf[0] << 0)  |
        (buf[1] << 8)  |
        (buf[2] << 16) |
        (buf[3] << 24)
    ) >>> 0; // force unsigned
}

export default class COF {
    constructor(v) {
        this.id=0;
        this.data=new Uint8Array(8);

        if (typeof v=="string")
            Object.assign(this,slcanParse(v));

        else if (v)
            Object.assign(this,v);
    }

    set(prop, value) {
        // COB-ID bitfields
        if (prop in COF.cobBits) {
            const f = COF.cobBits[prop];

            const buf = new Uint8Array(4);
            u32ToLe(this.id, buf);

            setBits(buf, f.offs, f.len, value);

            this.id = leToU32(buf) & 0x7FF; // clamp to 11 bits
            return;
        }

        // DATA bitfields
        if (prop in COF.dataBits) {
            const f = COF.dataBits[prop];

            if (f.len < 32) {
                value &= (1 << f.len) - 1;
            }

            setBits(this.data, f.offs, f.len, value);

            /*const maxByte = (f.offs + f.len + 7) >> 3;
            if (cof.len < maxByte) {
                cof.len = maxByte;
            }*/
            return;
        }

        // Direct properties
        switch (prop) {
            case "COF_COB_ID":
                cof.id = value;
                return;

            case "COF_DLC":
                throw new Error("WIP");
                //cof.len = value;
                return;
        }

        throw new Error(`write unknown prop: ${prop}`);
    }


    toSlcan(s) {
        return slcanStringify(this);
    }

    get(prop) {
        // COB-ID bitfields
        if (prop in COF.cobBits) {
            const f = COF.cobBits[prop];

            const buf = new Uint8Array(4);
            u32ToLe(this.id, buf);

            return getBits(buf, f.offs, f.len);
        }

        // DATA bitfields
        if (prop in COF.dataBits) {
            const f = COF.dataBits[prop];
            return getBits(this.data, f.offs, f.len);
        }

        // Direct properties
        switch (prop) {
            case "COF_COB_ID":
                return this.id;

            case "COF_DLC":
                return this.data.length;
        }

        throw new Error(`read unknown prop: ${prop}`);
    }
}

for (let k in cofSchema.defines)
    COF[k]=cofSchema.defines[k];

for (let k in cofSchema.dataBits)
    COF[k]=k;

for (let k in cofSchema.cobBits)
    COF[k]=k;

COF.dataBits=cofSchema.dataBits;
COF.cobBits=cofSchema.cobBits;

COF.sdoErrors={}
COF.sdoErrors[COF.COF_ABORT_TOGGLE]="Toggle bit not alternated";
COF.sdoErrors[COF.COF_ABORT_TIMEOUT]="SDO protocol timed out";
COF.sdoErrors[COF.COF_ABORT_COMMAND]="Command specifier not valid or unknown";
COF.sdoErrors[COF.COF_ABORT_OUT_OF_MEMORY]="Out of memory";
COF.sdoErrors[COF.COF_ABORT_UNSUPPORTED_ACCESS]="Unsupported access to an object";
COF.sdoErrors[COF.COF_ABORT_NOT_EXIST]="Object does not exist in OD";
COF.sdoErrors[COF.COF_ABORT_READONLY]="Attempt to write a read-only object";
COF.sdoErrors[COF.COF_ABORT_TYPE_MISMATCH]="Data type does not match";
COF.sdoErrors[COF.COF_ABORT_LENGTH_EXCEEDED]="Length of service parameter too high";
COF.sdoErrors[COF.COF_ABORT_SUBINDEX]="Subindex does not exist";
COF.sdoErrors[COF.COF_ABORT_DEVICE_STATE]="Device internal error";
