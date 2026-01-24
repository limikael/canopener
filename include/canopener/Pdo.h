#pragma once

namespace canopener {
	class Pdo {
	public:
		Pdo(int pdoNum_);
		void add(Entry& entry);
		void init();

	private:
		int pdoNum;
		EntryContainer *container=nullptr;
	    void setContainer(EntryContainer *container_) { this->container=container_; }

		friend class EntryContainer;
		friend class RemoteDevice;
	};
}