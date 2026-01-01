import COF from "../lib/cof.js";

describe("cof",()=>{
	it("can create and stringify a CANopen frame",()=>{
		let cof=new COF();

		cof.set(COF.COF_FUNC, COF.COF_FUNC_SDO_TX);
		cof.set(COF.COF_NODE_ID, 0x05);
		cof.set(COF.COF_SDO_CMD, COF.COF_SDO_SCS_ABORT);
		cof.set(COF.COF_SDO_INDEX, 0x4001);
		cof.set(COF.COF_SDO_SUBINDEX, 0x10);
		cof.set(COF.COF_SDO_ABORT_CODE, COF.COF_ABORT_NOT_EXIST);

		let s=cof.toSlcan();
		//console.log("here: ",s);
		expect(s).toEqual("t58588001401000000206");
	});

	it("it can parse a CANopen frame",()=>{
		let cof=new COF("t58588001401000000206");

		//console.log(COF.slcanStringify(cof));
		expect(cof.toSlcan(cof)).toEqual("t58588001401000000206");
		expect(cof.get(COF.COF_FUNC)).toEqual(COF.COF_FUNC_SDO_TX);
	});

	it("can parse heartbeat frames",()=>{
		let cof=new COF("t70617F");

		//console.log("cof type: "+cof.get(COF.COF_FUNC));
		expect(cof.get(COF.COF_FUNC)).toEqual(COF.COF_FUNC_HEARTBEAT);
	});
});