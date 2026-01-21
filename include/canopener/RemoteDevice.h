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
		bool isRefreshInProgress();

	private:
		void handleLoop();
		void handleMessage(cof_t *frame);
		int nodeId;
		MasterDevice *masterDevice=nullptr;
		Entry *sdoWriteEntry=nullptr;
		Entry *sdoReadEntry=nullptr;
		uint32_t sdoWriteDeadline=0;
		uint32_t sdoReadDeadline=0;
		int sdoWriteGeneration;
	};
}
