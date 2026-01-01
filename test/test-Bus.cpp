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
