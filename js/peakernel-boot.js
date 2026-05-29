RemoteDevice.prototype.awaitState=function(waitForState) {
	if (this.getState()==waitForState)
		return Promise.resolve();

	let that=this;
	return new Promise(function (resolve, reject) {
		function handleStateChange() {
			if (that.getState()==waitForState) {
				that.off("stateChange",handleStateChange);
				resolve();
			}
		}

		that.on("stateChange",handleStateChange);
	});
}

/*let originalFlush=RemoteDevice.prototype.flush;
RemoteDevice.prototype.flush=function() {
	let that=this;
	return new Promise(function (resolve,reject) {
		let flushPromise=originalFlush.bind(that)();
		function handleFlush() {
			flushPromise.off("flush",handleFlush);
			resolve();
		}

		flushPromise.on("flush",handleFlush);
	});
}*/

let originalGetConnectedDeviceIds=MasterDevice.prototype.getConnectedDeviceIds;
MasterDevice.prototype.getConnectedDeviceIds=function() {
	let intArray=originalGetConnectedDeviceIds.bind(this)();
	let a=[];
	for (let i=0; i<intArray.size(); i++)
		a.push(intArray.at(i));

	return a;
}

function getMasterDevice() {
	if (!globalThis.masterDevice)
		globalThis.masterDevice=new MasterDevice(getBus());

	return globalThis.masterDevice;
}
