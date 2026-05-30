#include <cstdio>
#include <cassert>
#include "async_primitives.hpp"

void test_Dispatcher() {
	printf("- disatcher...\n");

	Dispatcher<> d;
	bool called=false;

	d.on([&called](){
		called=true;
	});

	d.emit();
	assert(called);
}

int main() {
	printf("Running standalone tests...\n");

	test_Dispatcher();

	return 0;
}