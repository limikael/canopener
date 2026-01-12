#include <stdio.h>
#include <cassert>
#include "canopener.h"
#include <format>
#include <iostream>

using namespace canopener;

void test_MockBus() {
	printf("- Bus can send and receive...\n");
	MockBus bus;
	cof_t a,b;

	bus.rxBufPushSlcan("t123411223344");

	bus.read(&b);
	char s[16];
	cof_to_slcan(&b,s);
	assert(!strcmp(s,"t123411223344"));

	cof_set(&b,COF_COB_ID,0x55);

	bus.write(&b);
	a=bus.txBuf.front();
	cof_to_slcan(&a,s);
	assert(!strcmp(s,"t055411223344"));
}

void test_HubBus() {
	BusHub hub;
	Bus* a=hub.createBus();
	Bus* b=hub.createBus();

	cof_t cof,cof2;
	cof_from_slcan(&cof,"t123411223344");

	assert(!b->available());
	a->write(&cof);
	assert(b->available());

	b->read(&cof2);

	assert(cof_to_slcan_string(&cof2)=="t123411223344");

	std::string s=cof_to_slcan_string(&cof2);
	//printf("frame: %s\n",s.c_str());
}