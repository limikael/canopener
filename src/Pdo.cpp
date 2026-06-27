#include "canopener/Pdo.h"

using namespace canopener;

Pdo::Pdo(int pdoNum_) {
	pdoNum=pdoNum_;
}

void Pdo::add(std::shared_ptr<Entry> entry) {
    std::shared_ptr<Entry> pdoEntry=container->at(0x1A00+pdoNum-1,1);
    pdoEntry->setData(0,32);
    pdoEntry->setData(1,entry->getSubIndex());
    pdoEntry->setData(2,(entry->getIndex()&0xff));
    pdoEntry->setData(3,(entry->getIndex()>>8));
}

void Pdo::setInhibitTimeMs(uint16_t ms) {
    container->at(0x1800+pdoNum-1,3)->setUint(ms*10);
}

uint32_t Pdo::getInhibitTimeMs() {
    return container->at(0x1800+pdoNum-1,3)->getUint()/10;
}

void Pdo::init() {
    // Mapped application object 1
	container->insert(0x1A00+pdoNum-1,1);

    // Inhibit, in 100us
    container->insert(0x1800+pdoNum-1,3)->setType(Entry::UINT16);
}

int Pdo::getMappedIndex() {
    auto pdoMappingEntry=container->at(0x1A00+pdoNum-1,1);
    return (pdoMappingEntry->getData(2)+(pdoMappingEntry->getData(3)<<8));
}

int Pdo::getMappedSubIndex() {
    auto pdoMappingEntry=container->at(0x1A00+pdoNum-1,1);
    return pdoMappingEntry->getData(1);
}
