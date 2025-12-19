export function awaitEvent(target, type, processor) {
	return new Promise((resolve, reject)=>{
		function handle(...args) {
			try {
				let v=processor(...args);
				if (v) {
					target.off(type,handle);
					resolve(v);
				}
			}

			catch (e) {
				target.off(type,handle);
				reject(e);
			}
		}

		//console.log("adding: "+options.type);
		target.on(type,handle);
	});
}