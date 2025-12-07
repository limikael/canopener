import SlcanBus from "../lib/SlcanBus.js";

describe("SlcanBus",()=>{
	it("can spawn a command",()=>{
		let bus=new SlcanBus({spawn: "bin/mockdevice"});

		bus.send("t60582301403378563412"); // expidited SDO write
	});
});