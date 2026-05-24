#include "canopener/MasterDevice.h"
#include <stdio.h>

using namespace canopener;

MasterDevice::MasterDevice(std::shared_ptr<Bus> b) {
	device=std::make_shared<Device>(b);
	device->setNodeId(1);
	loopHandlerId=getBus()->loopDispatcher.on([this]() {
		handleLoop();
	});
	messageHandlerId=getBus()->messageDispatcher.on([this](cof_t *frame) {
		handleMessage(frame);
	});
};

MasterDevice::~MasterDevice() {
	getBus()->loopDispatcher.off(loopHandlerId);
	getBus()->messageDispatcher.off(messageHandlerId);
}

void MasterDevice::handleMessage(cof_t *frame) {
	if (cof_get(frame,COF_FUNC)==COF_FUNC_HEARTBEAT) {
		int nodeId=cof_get(frame,COF_NODE_ID);
		if (nodeId!=1) {
			uint32_t now=getBus()->millis();

		    for (auto& conn: remoteConnections) {
		        if (conn.getNodeId()==nodeId) {
		            conn.setHeartbeatDeadline(now+3000);
		            return;
		        }
		    }

			//printf("saw new node: %d\n",nodeId);
		    remoteConnections.push_back(RemoteConnection(nodeId));
		    remoteConnections.back().setHeartbeatDeadline(now+3000);

		    devicesChangedEvent.emit();
		}
	}
}

void MasterDevice::handleLoop() {
	bool changed=false;
	uint32_t now=getBus()->millis();
    for (auto it=remoteConnections.begin(); it!=remoteConnections.end(); ) {
        if (now>it->getHeartbeatDeadline()) {
			//printf("removing node: %d\n",it->getNodeId());
            it=remoteConnections.erase(it);  // returns next valid iterator
            changed=true;
        }

        else {
            it++;
        }
    }

    if (changed)
	    devicesChangedEvent.emit();
}

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

std::shared_ptr<IntArray> MasterDevice::getConnectedDeviceIds() {
	auto a=std::make_shared<IntArray>();
    for (auto& conn: remoteConnections)
    	a->push(conn.getNodeId());

	return a;
}