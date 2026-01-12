#pragma once
#include "Device.h"
#include "cof.h"

namespace canopener {
	class RemoteDevice;

	void handleSdoExpeditedRead(Device &dev, cof_t *frame);
	void handleSdoExpeditedWrite(Device &dev, cof_t *frame);

	void performSdoExpeditedWrite(RemoteDevice &dev, Entry &e);
}