export function awaitEvent(target, type, processor) {
	return new Promise((resolve, reject)=>{
		function handle(...args) {
			try {
				let v=processor(...args);
				if (v)
					resolve(v);
			}

			catch (e) {
				reject(e);
			}
		}

		//console.log("adding: "+options.type);
		target.on(type,handle);
	});
}