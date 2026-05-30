#pragma once
#include <vector>
#include <memory>
#include "Entry.h"
#include "Bus.h"
//#include "Dispatcher.h"
#include "EntryContainer.h"
#include "protocol.h"
#include "async_primitives.hpp"
#include "cof.h"

namespace canopener {
	class Device: public EntryContainer {
	public:
	    enum State {
	    	DISCONNECTED, OPERATIONAL
	    };

		Device(std::shared_ptr<Bus> b);
		~Device();
		int getNodeId() { return nodeId; }
		void setNodeId(int nodeId_) { nodeId=nodeId_; };
		std::shared_ptr<Bus> getBus() { return bus; };
		State getState() { return state; };
        void handleChange(std::shared_ptr<Entry> e) override;

	private:
		std::shared_ptr<Bus> bus;
		int handleLoopId,handleMessageId,handleLoopTimeoutId;
		void handleLoop();
		void handleMessage(cof_t *frame);
		void handleLoopTimeout(std::shared_ptr<LoopTimeoutEvent> ev);
		int nodeId;
		uint32_t heartbeatDeadline;
		uint32_t masterHeartbeatDeadline;
		uint32_t heartbeatInterval;
		State state;
		int segmentedUploadIndex=0, segmentedUploadSubIndex=0;
		bool segmentedUploadToggleBit=false;
		size_t segmentedUploadOffset;

	friend void handleUploadRequest(Device *dev, cof_t *frame);
	friend void handleSegmentedUpload(Device *dev, cof_t *frame);
	};
}
