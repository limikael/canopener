#include "canopener.h"

#ifdef ARDUINO
using namespace canopener;

SerialBus::SerialBus(Stream& stream)
		:stream(stream) {
}

bool SerialBus::available() {
	return rxBuf.size();
}

bool SerialBus::read(cof_t *frame) {
	cof_t c=rxBuf.front();
	cof_cpy(frame,&c);
	rxBuf.pop();
	return true;
}

void SerialBus::write(cof_t *frame) {
	char frames[256];
	if (cof_to_slcan(frame,frames)) {
		char s[256];
		sprintf(s,"%s\r\n",frames);
		stream.print(s);
		stream.flush();
	}
}

void SerialBus::loop() {
	if (stream.available()) {
		String s=stream.readStringUntil('\n');
		cof_t cof;

		//stream.println("read something...");

		if (cof_from_slcan(&cof,s.c_str())) {
			//printf("pushing...\n");
			rxBuf.push(cof);
		}
	}
}

#endif