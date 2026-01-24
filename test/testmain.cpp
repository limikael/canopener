#include <stdio.h>

void test_MockBus();
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
void test_bindings_listeners();
void test_bindings_types();
void test_remote_generations();
void test_pdo();
void test_remote_refresh();

int main() {
	printf("Running tests...\n");

	test_MockBus();
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
	test_bindings_listeners();
	test_pdo();
	test_remote_refresh();
	test_bindings_types();

	return 0;
}