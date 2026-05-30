#pragma once
#include "Device.h"
#include "cof.h"

namespace canopener {
	class RemoteDevice;
	class Device;

	void handleUploadRequest(Device *dev, cof_t *frame);
	void handleSdoExpeditedWrite(Device *dev, cof_t *frame);
	void handleSegmentedUpload(Device *dev, cof_t *frame);

	void performSdoExpeditedWrite(RemoteDevice *dev, Entry *e);
	void performSdoExpeditedRead(RemoteDevice *dev, Entry *e);
}