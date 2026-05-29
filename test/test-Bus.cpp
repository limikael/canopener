#include <stdio.h>
#include <cassert>
#include "canopener/MockBus.h"
#include "canopener/BusPair.h"
#include <format>
#include <iostream>

using namespace canopener;

void test_MockBus() {
	printf("- Mock Bus can send and receive...\n");
	MockBus bus;
	cof_t a,b;

	bus.writeSlcan("t123411223344");

	/*bus.read(&b);
	char s[16];
	cof_to_slcan(&b,s);
	assert(!strcmp(s,"t123411223344"));

	cof_set(&b,COF_COB_ID,0x55);

	bus.write(&b);

	a=bus.txBuf.front();
	cof_to_slcan(&a,s);
	assert(!strcmp(s,"t055411223344"));*/
}

void test_BusPair() {
	printf("- Bus Pair...\n");
	BusPair pair;

	pair.getSecond()->dataDispatcher.on([](uint32_t id, std::vector<uint8_t> data){
		//printf("got data, id=%d, len=%d\n",id,data.size());
		assert(id==123);
		assert(data[0]==1);
		assert(data[1]==2);
		assert(data[2]==3);
		assert(data[3]==4);
	});

	pair.getFirst()->writeData(123,std::vector<uint8_t>{1,2,3,4});
}

