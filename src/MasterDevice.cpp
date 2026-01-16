#include "canopener.h"

using namespace canopener;

RemoteDevice *MasterDevice::createRemoteDevice(int nodeId) {
	RemoteDevice *remoteDevice=new RemoteDevice(nodeId);
	remoteDevice->setMasterDevice(this);

	remoteDevices.push_back(remoteDevice);

	return remoteDevice;
}

RemoteDevice *MasterDevice::getRemoteDevice(int nodeId) {
	for (auto it: remoteDevices)
		if (it->getNodeId()==nodeId)
			return it;

	return nullptr;
}