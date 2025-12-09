#include "canopener.h"
#include <unistd.h>

using namespace canopener;

int main() {
	SlcanStdioBus bus(0,1);
	Device device(bus);
	device.setNodeId(5);

	device.insert(0x4001,0x33).setType(Entry::INT32);

	//for (int i=0; i<5; i++) {
		//printf("Started mockdevice...\r\n");
		fflush(stdout);
	/*	sleep(1);
	}*/

	while (1) {
		device.loop();
	}

	return 0;
}
