import COF from "./cof.js";
import {awaitEvent} from "./js-util.js";
import {slcanStringify} from "./SlcanBus.js";

export default class RemoteEntry {
	constructor(index, subIndex) {
		this.index=index;
		this.subIndex=subIndex;
		this.data=undefined;
	}

	async expeditedRead() {
		let cof=COF._cof_create();
		COF._cof_set(cof,COF.COF_TYPE,COF.COF_TYPE_SDO_UPLOAD_REQUEST);
        COF._cof_set(cof,COF.COF_NODE_ID,this.device.getNodeId()); 
        COF._cof_set(cof,COF.COF_SDO_INDEX,this.index); 
        COF._cof_set(cof,COF.COF_SDO_SUB,this.subIndex);
        let o=COF.cofToObject(cof);
        COF._cof_dispose(cof);

        this.device.bus.send(o);
        await awaitEvent(this.device.bus,"message",m=>{
			let cof=COF.cofFromObject(m);
			//console.log("t: "+COF._cof_get(cof,COF.COF_TYPE));

			if (COF._cof_get(cof,COF.COF_TYPE)==COF.COF_TYPE_SDO_UPLOAD_REPLY &&
					COF._cof_get(cof,COF.COF_SDO_INDEX)==this.index &&
					COF._cof_get(cof,COF.COF_SDO_SUB)==this.subIndex) {
				let size=COF._cof_get(cof,COF.COF_SDO_EXP_SIZE);
				/*console.log(slcanStringify(m));
				console.log("size: "+size);*/

		        COF._cof_dispose(cof);
				return true;
			}

			if (COF._cof_get(cof,COF.COF_TYPE)==COF.COF_TYPE_SDO_ABORT &&
					COF._cof_get(cof,COF.COF_SDO_INDEX)==this.index &&
					COF._cof_get(cof,COF.COF_SDO_SUB)==this.subIndex) {
				let e=COF.sdoErrors[COF._cof_get(cof,COF.COF_SDO_ABORT_CODE)];
		        COF._cof_dispose(cof);
		    	throw new Error(e);
			}

			else {
		        COF._cof_dispose(cof);
			}
        });
	}

	async expeditedWrite() {
		//console.log("doing expedited write...");
		let cof=COF._cof_create();
		COF._cof_set(cof,COF.COF_TYPE,COF.COF_TYPE_SDO_DOWNLOAD_REQUEST);
		COF._cof_set(cof,COF.COF_SDO_EXPEDITED,1);
		COF._cof_set(cof,COF.COF_SDO_SIZE_IND,1);
		COF._cof_set(cof,COF.COF_SDO_EXP_SIZE,4);
        COF._cof_set(cof,COF.COF_NODE_ID,this.device.getNodeId()); 
        COF._cof_set(cof,COF.COF_SDO_INDEX,this.index); 
        COF._cof_set(cof,COF.COF_SDO_SUB,this.subIndex);

        let p=COF._cof_getp(cof,COF.COF_SDO_EXP_DATA);
        COF.HEAPU8[p]=0x78;
        COF.HEAPU8[p+1]=0x56;
        COF.HEAPU8[p+2]=0x34;
        COF.HEAPU8[p+3]=0x12;

        let o=COF.cofToObject(cof);
        COF._cof_dispose(cof);

        this.device.bus.send(o);
        await awaitEvent(this.device.bus,"message",m=>{
			let cof=COF.cofFromObject(m);
			if (COF._cof_get(cof,COF.COF_TYPE)==COF.COF_TYPE_SDO_DOWNLOAD_REPLY &&
					COF._cof_get(cof,COF.COF_SDO_INDEX)==this.index &&
					COF._cof_get(cof,COF.COF_SDO_SUB)==this.subIndex) {
		        COF._cof_dispose(cof);
				return true;
			}

			if (COF._cof_get(cof,COF.COF_TYPE)==COF.COF_TYPE_SDO_ABORT &&
					COF._cof_get(cof,COF.COF_SDO_INDEX)==this.index &&
					COF._cof_get(cof,COF.COF_SDO_SUB)==this.subIndex) {
				let e=COF.sdoErrors[COF._cof_get(cof,COF.COF_SDO_ABORT_CODE)];
		        COF._cof_dispose(cof);
		    	throw new Error(e);
			}

			else {
		        COF._cof_dispose(cof);
			}
        });
	}

	async set(value) {
		this.data=value;
		return await this.expeditedWrite();
	}

	async refresh() {
		await this.expeditedRead();
	}
}