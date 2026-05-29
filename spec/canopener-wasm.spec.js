import {MasterDevice, MockBus, Device, BusPair} from "../js/exports-node.js";
import {importPython} from "pymod";
import {dirnameFromImportMeta} from "../js/node-util.js";
import path from "node:path";

let __dirname=dirnameFromImportMeta(import.meta);

async function importPythonDeviceMod(bus) {
	let ct=await importPython(path.join(__dirname,"canopener-test.py"),{uv: true});
	ct.on("message",(id, data)=>{
		bus.writeData(id,new Uint8Array(data.split(",")))
	});
	bus.on("data",async (id, data)=>{
		//console.log("sending back: ",id,data);
		await ct.message(id,data.toString());
	});

	return ct;
}

describe("canopener wasm build",()=>{
	it("doesn't smoke",async ()=>{
		let bus=new MockBus();
		bus.on("slcan",m=>{
			//console.log(m);
		});
		let m=new MasterDevice(bus);

		expect(bus.getTimeout()).toEqual(1000);
		bus.tickMockMillis(300);

		expect(bus.getTimeout()).toEqual(700);
		bus.tickMockMillis(700);

		bus.loop();
	});

	it("can interop with python canopen test1",async ()=>{
		let pair=new BusPair();
		let mod=await importPythonDeviceMod(pair.getSecond());

		let device=new Device(pair.getFirst());
		device.insert(0x2000,0).setInt(0x12345678);
		device.setNodeId(6);

		let v=await mod.test1();
		expect(v).toEqual(0x12345678);
		await mod.terminate();
	});
});