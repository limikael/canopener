import RemoteEntry from "./RemoteEntry.js";

export default class RemoteDevice {
	constructor({bus, nodeId}={}) {
		if (!bus || !nodeId)
			throw new Error("Need bus and node id!");

		this.bus=bus;
		this.entries=[];
		this.nodeId=nodeId;

		/*this.bus.on("message",()=>{
			console.log("message in bus!!");
		});*/
	}

	getNodeId() {
		if (!this.nodeId)
			throw new Error("No node id!");

		return this.nodeId;
	}

	entry(index, subIndex) {
		for (let entry of this.entries)
			if (entry.index==index && entry.subIndex==subIndex)
				return entry;

		let entry=new RemoteEntry(index,subIndex);
		entry.device=this;
		this.entries.push(entry);

		return entry;
	}
}