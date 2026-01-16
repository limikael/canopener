#pragma once
#include <vector>
#include "RemoteDevice.h"

namespace canopener {
	class MasterDevice {
	public:
		MasterDevice(Bus &b)
				:device(b) {
			device.setNodeId(1);
		};

		RemoteDevice *createRemoteDevice(int nodeId);
		Bus& getBus() { return device.getBus(); };

	private:
		Device device;
		std::vector<RemoteDevice*> remoteDevices;
	};
}
