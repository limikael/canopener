#pragma once
#include <vector>
#include "Entry.h"
#include "Bus.h"
#include "EntryContainer.h"

namespace canopener {
	class Device: public EntryContainer {
	public:
	    enum State {
	    	DISCONNECTED, OPERATIONAL
	    };

		Device(Bus& b);
		int getNodeId() { return nodeId; }
		void setNodeId(int nodeId_) { nodeId=nodeId_; };
		Bus& getBus() { return bus; };
		State getState() {return state; };

	private:
		void handleMessage(cof_t *frame);
		void handleLoop();
		Bus& bus;
		int nodeId;
		uint32_t heartbeatDeadline;
		uint32_t masterHeartbeatDeadline;
		uint32_t heartbeatInterval;
		State state;
	};
}
