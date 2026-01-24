#include "canopener.h"

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

Entry& EntryContainer::insert(uint16_t index, uint8_t subindex) {
	Entry *existing=find(index,subindex);
	if (existing)
		throw std::out_of_range("Entry already exists");

	Entry* e=new Entry(index,subindex);
	e->setContainer(this);
	entries.push_back(e);
	return *e;
}

Entry& EntryContainer::insert(uint16_t index) {
	return insert(index,0);
}

Entry& EntryContainer::at(uint16_t index, uint8_t subindex) {
	Entry *e=find(index,subindex);
	if (!e)
		throw std::out_of_range("Entry not found");

	return *e;
}

Entry& EntryContainer::at(uint16_t index) {
	return at(index,0);
}

Entry* EntryContainer::find(uint16_t index, uint8_t subindex) {
	for (Entry* e: entries)
		if (e->index==index && subindex==e->subindex)
			return e;

	return NULL;
}

int EntryContainer::getGeneration() {
	int generation=0;

	for (Entry* e: entries) {
		if (e->generation>generation)
			generation=e->generation;
	}

	return generation;
}

int EntryContainer::getCommitGeneration() {
	int commitGeneration=getGeneration();

	for (Entry* e: entries) {
		if (e->commitGeneration!=e->generation &&
				e->commitGeneration<commitGeneration)
			commitGeneration=e->commitGeneration;
	}

	return commitGeneration;
}

Pdo& EntryContainer::pdo(int pdoNum) {
	return *pdos[pdoNum-1];
}