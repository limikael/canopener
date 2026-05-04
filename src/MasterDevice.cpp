#include "canopener/MasterDevice.h"

using namespace canopener;

MasterDevice::MasterDevice(std::shared_ptr<Bus> b) {
	device=std::make_shared<Device>(b);
	device->setNodeId(1);
};

std::shared_ptr<RemoteDevice> MasterDevice::createRemoteDevice(int nodeId) {
	std::shared_ptr<RemoteDevice> remoteDevice=std::make_shared<RemoteDevice>(nodeId);
	remoteDevice->setMasterDevice(this);

	remoteDevices.push_back(remoteDevice);

	return remoteDevice;
}

std::shared_ptr<RemoteDevice> MasterDevice::getRemoteDevice(int nodeId) {
	for (auto it: remoteDevices)
		if (it->getNodeId()==nodeId)
			return it;

	return nullptr;
}