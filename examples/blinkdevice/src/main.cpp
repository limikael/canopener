#include <Arduino.h>
#include "canopener.h"
using namespace canopener;

EspBus espBus(5,4); // tx, rx def= 5,4
//SerialBus serialBus(Serial);
//BridgeBus bus(serialBus,espBus);
Device dev(espBus);

void setup() {
    Serial.begin(115200);
    dev.setNodeId(DEVID);
    dev.insert(0x2000,0)/*.setType(Entry::BOOL)*/.set<bool>(false);

    pinMode(8,OUTPUT);
    pinMode(0,OUTPUT);

    pinMode(10,INPUT_PULLUP);
    pinMode(20,INPUT_PULLUP);
    pinMode(1,INPUT_PULLUP);
    pinMode(3,INPUT_PULLUP);

    espBus.messageDispatcher.on([](cof_t *frame) {
        Serial.printf("read: 0x%03x [%d]:",frame->id,frame->len);
        for (int i=0; i<frame->len; i++)
            Serial.printf(" %02x",frame->data[i]);

        Serial.printf("\n");
    });
}

void loop() {
	espBus.loop();
    digitalWrite(8,dev.at(0x2000,0).get<bool>());

    delay(250);
    Serial.printf("input: %d %d %d %d\n",
        digitalRead(10),
        digitalRead(20),
        digitalRead(3),
        digitalRead(1)
    );

    digitalWrite(0,!digitalRead(0));
}
