#include <stdio.h>

void test_MockBus();
void test_BridgeBus();
void test_DataView();
void test_cof();
void test_Device_basic();
void test_Device_expedited_write();
void test_Device_expedited_write16();
void test_Device_expedited_read();
void test_castx();
void test_HubBus();
void test_remote();
void test_bindings();
void test_remote_generations();

int main() {
	printf("Running tests...\n");

	test_MockBus();
	//test_BridgeBus();
	test_DataView();
	test_cof();
	test_Device_basic();
	test_Device_expedited_write();
	test_Device_expedited_write16();
	test_Device_expedited_read();
	test_castx();
	test_remote();
	test_remote_generations();
	test_bindings();

	return 0;
}