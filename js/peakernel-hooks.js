import {dirnameFromImportMeta} from "./node-util.js";
import path from "node:path";

let __dirname=dirnameFromImportMeta(import.meta);

export function build(ev) {
	ev.addIncludeDir(path.join(__dirname,"../include"));
	ev.addBinding(path.join(__dirname,"bindings.json"));
	ev.addSource(path.join(__dirname,"../src/EspBus.cpp"));
	ev.addSource(path.join(__dirname,"../src/cof.cpp"));
	ev.addSource(path.join(__dirname,"../src/pk-canopener.cpp"));
	ev.addSource(path.join(__dirname,"../src/cof-defines.cpp"));
	ev.addLoopFunction("canopener_loop");
	ev.addDefine("PEAKERNEL");
}