#pragma once
#include <vector>
#include <memory>
#include "Entry.h"
#include "Bus.h"
#include "Dispatcher.h"
#include "EntryContainer.h"
#include "Protocol.h"

namespace canopener {
	class Device: public EntryContainer {
	public:
	    enum State {
	    	DISCONNECTED, OPERATIONAL
	    };

		Device(std::shared_ptr<Bus> b);
		int getNodeId() { return nodeId; }
		void setNodeId(int nodeId_) { nodeId=nodeId_; };
		std::shared_ptr<Bus> getBus() { return bus; };
		State getState() {return state; };

	private:
		std::shared_ptr<Bus> bus;
		//Bus& bus;
		void handleMessage(cof_t *frame);
		void handleLoop();
		int nodeId;
		uint32_t heartbeatDeadline;
		uint32_t masterHeartbeatDeadline;
		uint32_t heartbeatInterval;
		State state;
	};
}
