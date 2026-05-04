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
		bool isComplete();
		std::shared_ptr<FlushPromise> getFlushPromise() { return flushPromise; }

	private:
		uint32_t deadline;
		bool initialized=false, complete=false;
		Type type;
		RemoteDevice *remoteDevice=nullptr;
		std::shared_ptr<Entry> entry;
		std::shared_ptr<FlushPromise> flushPromise;
	};
}
