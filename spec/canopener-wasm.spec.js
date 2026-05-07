import {MasterDevice, MockBus} from "../js/exports-node.js";

describe("canopener wasm build",()=>{
	it("doesn't smoke",()=>{
		let bus=new MockBus();
		let m=new MasterDevice(bus);

		console.log("***** created!!!!");
	});
});