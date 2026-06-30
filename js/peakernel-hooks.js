import {dirnameFromImportMeta} from "./node-util.js";
import path from "node:path";
import fs from "node:fs";

let __dirname=dirnameFromImportMeta(import.meta);

export function build(ev) {
	ev.addIncludeDir(path.join(__dirname,"../include"));
	ev.addBinding(path.join(__dirname,"bindings.json"));

	for (let fn of fs.readdirSync(path.join(__dirname,"../src")))
		ev.addSource(path.join(__dirname,"../src",fn));

	ev.addLoopFunction("canopener_loop");
	ev.addDefine("PEAKERNEL");

	ev.addBootFile(path.join(__dirname,"peakernel-boot.js"));

	if (process.env.PEAKERNEL_CAN_PINS) {
		let pins=process.env.PEAKERNEL_CAN_PINS.split(",").map(n=>Number(n));
		console.log("Using CAN pins: TX="+pins[0]+" RX="+pins[1]);
		ev.addBootContent(`
			initEspBus(${pins[0]},${pins[1]});
		`);
	}
}