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

let originalFlush=RemoteDevice.prototype.flush;
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
}