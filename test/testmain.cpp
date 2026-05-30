#include <stdio.h>

void test_MockBus();
void test_DataView();
void test_cof();
void test_Device_basic();
void test_Device_expedited_write();
void test_Device_expedited_write16();
void test_Device_expedited_read();
//void test_Device_segmented_read();
void test_castx();
void test_HubBus();
void test_remote();
void test_bindings();
void test_remote_flush();
void test_remote_segmented_read();
void test_pdo();
void test_remote_refresh();
void test_EntryContainer_basic();
void test_EntryContainer_notifications();
void test_MasterDevice_connectedDeviceIds();
void test_BusPair();

int main() {
	printf("Running tests...\n");

	test_EntryContainer_basic();
	test_EntryContainer_notifications();
	test_Device_basic();
	test_Device_expedited_write();
	test_Device_expedited_write16();
	test_Device_expedited_read();
	//test_Device_segmented_read();
	test_MockBus();
	test_BusPair();
	test_DataView();
	test_cof();
	test_castx();
	test_remote();
	test_remote_refresh();
	test_remote_flush();
	test_remote_segmented_read();
	test_pdo();
	test_MasterDevice_connectedDeviceIds();
	test_bindings();

	return 0;
}