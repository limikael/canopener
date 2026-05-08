function createProfileEntries(device, profile={}) {
	if (profile.entries) {
		for (let k in profile.entries) {
			let e=profile.entries[k];
			//console.log("e: "+k+" i: "+e.index+" s: "+e.subIndex);
			device.insert(e.index,e.subIndex);
		}
	}

	return device;
}

function createProfileProperties(device, profile={}) {
    const originalAt=device.at;
    function deviceAt(index,subIndex) {
    	return originalAt.call(device,index,subIndex);
    }

    device.at=function(index, subIndex) {
    	if (profile &&
    			profile.entries && 
    			profile.entries[index]) {
    		let e=profile.entries[index];
    		return deviceAt(e.index,e.subIndex);
    	}

    	return deviceAt(index,subIndex);
    };

	if (profile.entries) {
		for (let k in profile.entries) {
			let e=profile.entries[k];
			Object.defineProperty(device,k,{
				get() {
					return deviceAt(e.index,e.subIndex).getInt();
				},

				set(v) {
					deviceAt(e.index,e.subIndex).setInt(v);
				}
			});
		}
	}
}

export function openDevice(deviceId, deviceProfile) {
	let masterDevice=globalThis.getMasterDevice();
	let remoteDevice=masterDevice.createRemoteDevice(deviceId);
	createProfileEntries(remoteDevice,deviceProfile);
	createProfileProperties(remoteDevice,deviceProfile);
	Object.freeze(remoteDevice);

	return remoteDevice;
}
