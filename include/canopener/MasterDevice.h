#pragma once
#include <vector>
#include "RemoteDevice.h"
#include "RemoteConnection.h"
#include "Device.h"
#include "IntArray.h"

namespace canopener {
	class MasterDevice {
	public:
		MasterDevice(std::shared_ptr<Bus> b);
		~MasterDevice();
		std::shared_ptr<RemoteDevice> createRemoteDevice(int nodeId);
		std::shared_ptr<RemoteDevice> getRemoteDevice(int nodeId);
		std::shared_ptr<Bus> getBus() { return device->getBus(); }
		std::shared_ptr<IntArray> getConnectedDeviceIds();

        Dispatcher<> devicesChangedEvent;

	private:
		int loopHandlerId=0,messageHandlerId=0;
		void handleLoop();
		void handleMessage(cof_t *frame);
		std::shared_ptr<Device> device;
		std::vector<std::shared_ptr<RemoteDevice>> remoteDevices;
		std::vector<RemoteConnection> remoteConnections;
	};
}
