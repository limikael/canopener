#pragma once
#include "Bus.h"
#include "cof.h"
#include <queue>
#include <string>
#include <vector>

namespace canopener {
	class MockBus: public Bus {
	public:
		void write(cof_t *frame) override { 
			char s[256];
			cof_to_slcan(frame,s);
			log.push_back(std::string(s));
			messageDispatcher.emit(frame);
			slcanDispatcher.emit(s);
		};

		void tickMockMillis(int tickMillis) { mockMillis+=tickMillis; }

        uint32_t millis() override { return mockMillis; }
		uint32_t mockMillis=0;

		std::vector<std::string> log;
	};
}
