#pragma once
#include "RemoteDevice.h"
#include "Entry.h"

namespace canopener {
	class RemoteDevice;
	class FlushPromise;

	class RemoteCmd {
	public:
	    enum Type {
	    	SDO_WRITE,
	    	SDO_READ,
	    	FLUSH
	    };

		RemoteCmd(Type t, std::shared_ptr<Entry> e);
		RemoteCmd(Type t);
		void setRemoteDevice(RemoteDevice *d) { remoteDevice=d; }
		void handleLoop();
		void handleMessage(cof_t *frame);
		bool isInitialized() { return initialized; }
		bool isComplete();
		VoidPromise getFlushPromise() { return flushPromise; }
		Type getType() { return type; }
		std::shared_ptr<Entry> getEntry() { return entry; }

	private:
		uint32_t deadline;
		bool initialized=false, complete=false;
		Type type;
		RemoteDevice *remoteDevice=nullptr;
		std::shared_ptr<Entry> entry;
		VoidPromise flushPromise;
		bool segmentedUpload=false;
		bool segmentedToggleBit=false;
		size_t segmentedUploadOffset;
	};
}
