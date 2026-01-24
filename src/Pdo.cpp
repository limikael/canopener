#include "canopener.h"

using namespace canopener;

Pdo::Pdo(int pdoNum_) {
	pdoNum=pdoNum_;
}

void Pdo::add(Entry& entry) {
    Entry &pdoEntry=container->at(0x1A00+pdoNum-1,1);
    pdoEntry.setData(0,32);
    pdoEntry.setData(1,entry.getSubIndex());
    pdoEntry.setData(2,(entry.getIndex()&0xff));
    pdoEntry.setData(3,(entry.getIndex()>>8));
}