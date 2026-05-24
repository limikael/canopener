#pragma once
#include <cstdint>

namespace canopener {
	class RemoteConnection {
	public:
		RemoteConnection(int nodeId_) { nodeId=nodeId_; }
		int getNodeId() { return nodeId; }
		void setHeartbeatDeadline(uint32_t v) { heartbeatDeadline=v; }
		uint32_t getHeartbeatDeadline() { return heartbeatDeadline; }

	private:
		int nodeId;
		uint32_t heartbeatDeadline;
	};
}
