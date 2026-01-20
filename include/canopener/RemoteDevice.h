#pragma once
#include <vector>
#include "MasterDevice.h"
#include "Dispatcher.h"

namespace canopener {
	class MasterDevice;

	class RemoteDevice: public EntryContainer {
	public:
		RemoteDevice(int nodeId_);
		int getNodeId() { return nodeId; }
		Bus& getBus();
		void setMasterDevice(MasterDevice *masterDevice_);
		Dispatcher<> commitGenerationChangeDispatcher;

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
