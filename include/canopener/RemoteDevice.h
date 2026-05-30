#pragma once
#include <vector>
#include "MasterDevice.h"
#include "EntryContainer.h"
#include "Bus.h"
#include "RemoteCmd.h"
#include "async_primitives.hpp"

namespace canopener {
	class MasterDevice;
	class RemoteCmd;

	class RemoteDevice: public EntryContainer {
	public:
	    enum State {
	    	DISCONNECTED, OPERATIONAL
	    };

		RemoteDevice(int nodeId_);
		~RemoteDevice();
		int getNodeId() { return nodeId; }
		std::shared_ptr<Bus> getBus();
		void setMasterDevice(MasterDevice *masterDevice_);
        void handleChange(std::shared_ptr<Entry> e) override;
        void handleRefresh(std::shared_ptr<Entry> e) override;
        VoidPromise flush();
        std::string getState();

        Dispatcher<std::string> stateChangeEvent;

	private:
		int loopHandlerId=0,messageHandlerId=0;
		void handleLoop();
		void handleMessage(cof_t *frame);
		int nodeId;
		MasterDevice *masterDevice=nullptr;
		std::vector<std::shared_ptr<RemoteCmd>> cmds;
		std::shared_ptr<RemoteCmd> cmdInFlight;
		State state;
		uint32_t heartbeatDeadline;
	};
}
