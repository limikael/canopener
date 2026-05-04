#pragma once
#include <vector>
#include "MasterDevice.h"
#include "Dispatcher.h"
#include "EntryContainer.h"
#include "Bus.h"
#include "RemoteCmd.h"

namespace canopener {
	class MasterDevice;
	class RemoteCmd;

	class FlushPromise {
	public:
		Dispatcher<> flushEvent;
	};

	class RemoteDevice: public EntryContainer {
	public:
		RemoteDevice(int nodeId_);
		int getNodeId() { return nodeId; }
		std::shared_ptr<Bus> getBus();
		void setMasterDevice(MasterDevice *masterDevice_);
        void handleChange(std::shared_ptr<Entry> e) override;
        void handleRefresh(std::shared_ptr<Entry> e) override;
        std::shared_ptr<FlushPromise> flush();

	private:
		void handleLoop();
		void handleMessage(cof_t *frame);
		int nodeId;
		MasterDevice *masterDevice=nullptr;
		std::vector<std::shared_ptr<RemoteCmd>> cmds;
		std::shared_ptr<RemoteCmd> cmdInFlight;
	};
}
