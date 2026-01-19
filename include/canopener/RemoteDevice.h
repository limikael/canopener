#pragma once
#include <vector>
#include "MasterDevice.h"

namespace canopener {
	class MasterDevice;

	class RemoteDevice: public EntryContainer {
	public:
		RemoteDevice(int nodeId_) { nodeId=nodeId_; }
		int getNodeId() { return nodeId; }
		Bus& getBus();
		void setMasterDevice(MasterDevice *masterDevice_);

	private:
		void handleLoop();
		void handleMessage(cof_t *frame);
		int nodeId;
		MasterDevice *masterDevice=nullptr;
		Entry *sdoWriteEntry=nullptr;
		uint32_t sdoWriteDeadline=0;
		int sdoWriteGeneration;
	};
}
