#pragma once
#include <memory>
#include "canopener/cof.h"

namespace canopener {
	class Device;

	class DeviceSegmentedOp {
	public:
		static std::unique_ptr<DeviceSegmentedOp> createUpload(Device *dev_, int idx_, int sub_);
		void sendUploadResponse();
		void handleMessage(cof_t *frame);
		bool isComplete() { return complete; }

	private:
		DeviceSegmentedOp() {}
		Device *device;
		int index,subIndex;
		bool toggleBit=false;
		int offset=0;
		bool complete=false;
	};
}
