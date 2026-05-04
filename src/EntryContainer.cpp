#include "canopener/EntryContainer.h"
#include "canopener/Pdo.h"
#include "canopener/Entry.h"
#include <stdexcept>

using namespace canopener;

EntryContainer::EntryContainer() {
	for (int i=0; i<4; i++) {
		Pdo* pdo=new Pdo(i+1);
		pdo->setContainer(this);
		pdos.push_back(pdo);
		pdo->init();
	}

	//printf("container ctor\n");
}

std::shared_ptr<Entry> EntryContainer::insert(uint16_t index, uint8_t subindex) {
	std::shared_ptr<Entry> existing=find(index,subindex);
	if (existing)
		throw std::out_of_range("Entry already exists");

	//printf("creating...\n");
	std::shared_ptr<Entry> e=Entry::create(index,subindex);
	e->setContainer(this);
	entries.push_back(e);
	return e;
}

std::shared_ptr<Entry> EntryContainer::insert(uint16_t index) {
	return insert(index,0);
}

std::shared_ptr<Entry> EntryContainer::at(uint16_t index, uint8_t subindex) {
	std::shared_ptr<Entry> e=find(index,subindex);
	if (!e)
		throw std::out_of_range("Entry not found");

	return e;
}

std::shared_ptr<Entry> EntryContainer::at(uint16_t index) {
	return at(index,0);
}

std::shared_ptr<Entry> EntryContainer::find(uint16_t index, uint8_t subindex) {
	for (std::shared_ptr<Entry> e: entries)
		if (e->index==index && subindex==e->subindex)
			return e;

	return NULL;
}

Pdo& EntryContainer::pdo(int pdoNum) {
	return *pdos[pdoNum-1];
}