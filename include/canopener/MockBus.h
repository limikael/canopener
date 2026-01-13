#pragma once
#include "Bus.h"
#include "cof.h"
#include <queue>
#include <string>
#include <vector>

namespace canopener {
	class MockBus: public Bus {
	public:
		void write(cof_t *frame) { 
			char s[256];
			cof_to_slcan(frame,s);
			log.push_back(std::string(s));
			messageDispatcher.emit(frame);
		};

		void writeSlcan(std::string s) {
			cof_t frame;
			cof_from_slcan(&frame,s.c_str());
			write(&frame);
		}

        uint32_t millis() { return mockMillis; }
		uint32_t mockMillis=0;

		std::vector<std::string> log;
	};
}
