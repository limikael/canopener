#!/usr/bin/env node
import {promises as fsp} from "node:fs";

let object={};
let content=await fsp.readFile("include/canopener/cof.h","utf8");
for (let line of content.split("\n")) {
	if (line.startsWith("#define")) {
		let parts=line.split(/\s+/g);
		if (parts[2])
			object[parts[1]]=parseInt(parts[2]);
	}
}

//console.log(object);
await fsp.writeFile("dist/cof-defines.js","export default "+JSON.stringify(object,null,2));
