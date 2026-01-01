#!/usr/bin/env node
import {promises as fsp} from "node:fs";
import JSON5 from 'json5'
import fs from "fs";
import schema from "../lib/cof-schema.js";

let header="";
header+=`#pragma once\n\n`;
header+=`#ifdef __cplusplus\nextern "C" {\n#endif\n\n`;

for (let k in schema.defines)
	header+=`#define ${k} ${schema.defines[k]}\n`

header+="\n";

for (let [i, key] of Object.keys(schema.dataBits).entries())
	header+=`#define ${key} ${1000+i}\n`

header+="\n";

for (let [i, key] of Object.keys(schema.cobBits).entries())
	header+=`#define ${key} ${2000+i}\n`

header+=`
typedef struct {
    int offs, len;
} cof_field_t;

extern cof_field_t cof_data_bits[],cof_cob_bits[];

`;

header+=`#ifdef __cplusplus\n}\n#endif\n`;
//console.log(header);
fs.writeFileSync("include/canopener/cof-defines.h",header);

let source="";
source+=`#include "canopener/cof-defines.h"\n\n`;

source+="cof_field_t cof_data_bits[]={\n";
for (let [k,v] of Object.entries(schema.dataBits))
	source+=`    {.offs=${v.offs}, .len=${v.len}}, // ${k},\n`

source+="};\n\n";

source+="cof_field_t cof_cob_bits[]={\n";
for (let [k,v] of Object.entries(schema.cobBits))
	source+=`    {.offs=${v.offs}, .len=${v.len}}, // ${k},\n`

source+="};\n";

//console.log(source);
fs.writeFileSync("src/cof-defines.cpp",source);

