#include <Arduino.h>
#include <SoftTimer.h>
#include "canopener.h"

using namespace canopener;

SoftTimer timer(1000);
EspBus bus(5,4);

void setup() {
	Serial.begin(115200);
	pinMode(8,OUTPUT);

	bus.messageDispatcher.on([](cof_t *message) {
		char s[256];
		cof_to_slcan(message,s);

		Serial.printf("msg: %s...\n",s);
	});
}

void loop() {
	bus.loop();

	if (timer.tick()) {
		char s[256];
		sprintf(s,"t1232%02x%02x",rand()%255,rand()%255);
		Serial.printf("tick, write=%s\n",s);

		cof_t message;
		cof_from_slcan(&message,s); //"t1232AABB");
		bus.write(&message);

		digitalWrite(8,!digitalRead(8));
	}
}
