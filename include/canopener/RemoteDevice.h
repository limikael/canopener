#pragma once
#include <vector>
#include "MasterDevice.h"

namespace canopener {
	class MasterDevice;

	class RemoteDevice {
	public:
		RemoteDevice(int nodeId_) { nodeId=nodeId_; }
		Entry& insert(uint16_t index, uint8_t subindex);
		Entry& insert(uint16_t index);
		Entry& at(uint16_t index, uint8_t subindex);
		Entry& at(uint16_t index);
		Entry *find(uint16_t index, uint8_t subindex);
		int getNodeId() { return nodeId; }
		Bus& getBus();
		void setMasterDevice(MasterDevice *masterDevice_);

	private:
		void handleLoop();
		void handleMessage(cof_t *frame);
		int nodeId;
		MasterDevice *masterDevice=nullptr;
		std::vector<Entry*> entries;
		Entry *sdoWriteEntry=nullptr;
		uint32_t sdoWriteDeadline=0;
	};
}
