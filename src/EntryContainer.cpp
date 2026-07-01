#include "canopener/EntryContainer.h"
#include "canopener/Pdo.h"
#include "canopener/Entry.h"
#include <stdexcept>
#include <cassert>

using namespace canopener;

EntryContainer::EntryContainer() {
	changesSuppressed=false;
	for (int i=0; i<4; i++) {
		auto pdo=std::make_shared<Pdo>(i+1);
		pdo->setContainer(this);
		pdos.push_back(pdo);
		pdo->init();
	}
}

bool EntryContainer::popChangeNotificationSuppression() {
	bool b=changesSuppressed;
	changesSuppressed=false;
	return b;
}

void EntryContainer::suppressChangeNotification() {
	assert(!changesSuppressed);
	changesSuppressed=true;
}

std::shared_ptr<Entry> EntryContainer::insert(uint16_t index, uint8_t subindex) {
	std::shared_ptr<Entry> existing=find(index,subindex);
	if (existing) {
        #if defined(__cpp_exceptions)
            throw std::out_of_range("Entry already exists");
        #else
            assert(false && "entry already exists");
            std::abort(); // if asserts are disabled
        #endif
	}

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
	if (!e) {
        #if defined(__cpp_exceptions)
            throw std::out_of_range("Entry not found");
        #else
            assert(false && "Entry not found");
            std::abort(); // if asserts are disabled
        #endif
	}

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

std::shared_ptr<Pdo> EntryContainer::pdo(int pdoNum) {
	return pdos[pdoNum-1];
}

void EntryContainer::handleChange(std::shared_ptr<Entry> e) {
	//assert(0 && "handle change in base EntryContainer shouldn't be called");
}

void EntryContainer::handleRefresh(std::shared_ptr<Entry> e) {
}
