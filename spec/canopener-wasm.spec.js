import {MasterDevice, mockBusCreate} from "../js/exports-node.js";

describe("canopener wasm build",()=>{
	it("doesn't smoke",()=>{
		let bus=mockBusCreate();
		let m=new MasterDevice(bus);
	});
});