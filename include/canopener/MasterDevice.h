#pragma once
#include <vector>
#include "RemoteDevice.h"
#include "Device.h"

namespace canopener {
	class MasterDevice {
	public:
		MasterDevice(std::shared_ptr<Bus> b);
		std::shared_ptr<RemoteDevice> createRemoteDevice(int nodeId);
		std::shared_ptr<RemoteDevice> getRemoteDevice(int nodeId);
		std::shared_ptr<Bus> getBus() { return device->getBus(); };

	private:
		std::shared_ptr<Device> device;
		std::vector<std::shared_ptr<RemoteDevice>> remoteDevices;
	};
}
