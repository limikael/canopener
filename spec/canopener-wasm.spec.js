import {MasterDevice, MockBus} from "../js/exports-node.js";
import {importPython} from "pymod";
import {dirnameFromImportMeta} from "../js/node-util.js";
import path from "node:path";

let __dirname=dirnameFromImportMeta(import.meta);

describe("canopener wasm build",()=>{
	it("doesn't smoke",async ()=>{
		let ct=await importPython(path.join(__dirname,"canopener-test.py"));
		let v=await ct.hello();
		expect(v).toEqual(1234);

		let bus=new MockBus();
		bus.on("slcan",m=>{
			console.log(m);
		});
		let m=new MasterDevice(bus);

		bus.tickMockMillis(1000);

		bus.loop();
	});
});