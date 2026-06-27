#pragma once

#include "canopener/Entry.h"
#include "canopener/EntryContainer.h"
#include <memory>

namespace canopener {
	class Pdo {
	public:
		Pdo(int pdoNum_);
		void add(std::shared_ptr<Entry> entry);
		void init();
		int getPdoNum() { return pdoNum; }
		void setInhibitTimeMs(uint16_t ms);
		uint32_t getInhibitTimeMs();
		int getMappedIndex();
		int getMappedSubIndex();

	private:
		int pdoNum;
		EntryContainer *container=nullptr;
	    void setContainer(EntryContainer *container_) { this->container=container_; }
	    bool dirtyOutgoing=false;
	    uint32_t lastSend=0;

		friend class EntryContainer;
		friend class RemoteDevice;
		friend class Device;
	};
}