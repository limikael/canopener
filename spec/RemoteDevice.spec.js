import {openSlcanBus, RemoteDevice} from "../lib/exports-node.js";

describe("RemoteDevice",()=>{
	it("expedited write",async ()=>{
		let bus=await openSlcanBus({spawn: "bin/mockdevice"});
		let device=new RemoteDevice({bus: bus, nodeId: 5});
		let e=device.entry(0x4001,0x33);
		await e.set(0x12345678);

		expect(device.entry(0x4001,0x33).get()).toEqual(0x12345678);

		let e2=device.entry(0x4001,0x34);
		await expectAsync(e2.set(0x12345678)).toBeRejectedWith(new Error("Object does not exist in OD"));
	});

	it("expedited read",async ()=>{
		let bus=await openSlcanBus({spawn: "bin/mockdevice"});

		let device1=new RemoteDevice({bus: bus, nodeId: 5});
		let e1=device1.entry(0x4001,0x33);
		await e1.set(11223344);

		let device2=new RemoteDevice({bus: bus, nodeId: 5});
		let e2=device2.entry(0x4001,0x33);
		await e2.refresh();
		expect(e2.get()).toEqual(11223344);

		let e3=device2.entry(0x4001,0x34);
		await expectAsync(e3.refresh()).toBeRejectedWith(new Error("Object does not exist in OD"));
	});
});