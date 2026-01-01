#include <stdio.h>
#include "canopener.h"
#include <string>
#include <cassert>

using namespace canopener;

void test_BridgeBus() {
	printf("- Bridge Bus...\n");
	MockBus a,b;
	BridgeBus bridge(a,b);
	cof_t cof;
	char s[256];

	cof_init(&cof);
	cof_set(&cof,COF_COB_ID,0x123);
	cof_set(&cof,COF_DLC,8);
	bridge.write(&cof);
	assert(a.txBufPopSlcan()=="t12380000000000000000");
	assert(b.txBufPopSlcan()=="t12380000000000000000");

	assert(!a.available());
	assert(!b.available());
	assert(!bridge.available());

	a.rxBufPushSlcan("t123411223344");
	assert(a.available());
	assert(!b.available());
	assert(bridge.available());

	bridge.read(&cof);
	cof_to_slcan(&cof,s);
	assert(std::string(s)=="t123411223344");

	assert(!a.available());
	assert(b.txBufPopSlcan()=="t123411223344");
}
