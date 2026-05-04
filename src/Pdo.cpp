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

void Pdo::init() {
	container->insert(0x1A00+pdoNum-1,1);
}