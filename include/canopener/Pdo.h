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

	private:
		int pdoNum;
		EntryContainer *container=nullptr;
	    void setContainer(EntryContainer *container_) { this->container=container_; }

		friend class EntryContainer;
		friend class RemoteDevice;
	};
}