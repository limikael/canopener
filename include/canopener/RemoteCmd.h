#pragma once
#include "RemoteDevice.h"
#include "Entry.h"

namespace canopener {
	class RemoteDevice;

	class RemoteCmd {
	public:
	    enum Type {
	    	SDO_WRITE,
	    	SDO_READ
	    };

		RemoteCmd(Type t, std::shared_ptr<Entry> e);
		void setRemoteDevice(RemoteDevice *d) { remoteDevice=d; }
		void handleLoop();
		void handleMessage(cof_t *frame);
		bool isComplete();

	private:
		uint32_t deadline;
		bool initialized=false, complete=false;
		Type type;
		RemoteDevice *remoteDevice=nullptr;
		std::shared_ptr<Entry> entry;
	};
}
