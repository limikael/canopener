#include <Arduino.h>
#include "canopener.h"
#include "DebouncePin.h"

using namespace canopener;

EspBus espBus(5,4); // tx, rx def= 5,4
Device dev(espBus);

const int numInputPins=4;
DebouncePin inputPins[4]={
    DebouncePin(10),
    DebouncePin(20),
    DebouncePin(3),
    DebouncePin(1),
};

void setup() {
    Serial.begin(115200);
    dev.setNodeId(DEVID);
    dev.insert(0x2000,0)/*.setType(Entry::BOOL)*/.set<bool>(false);
    dev.insert(0x6400,0).set<int>(4);
    dev.insert(0x6400,1);
    dev.insert(0x6400,2);
    dev.insert(0x6400,3);
    dev.insert(0x6400,4);

    pinMode(8,OUTPUT);
    pinMode(0,OUTPUT);

    espBus.messageDispatcher.on([](cof_t *frame) {
        Serial.printf("read: 0x%03x [%d]:",frame->id,frame->len);
        for (int i=0; i<frame->len; i++)
            Serial.printf(" %02x",frame->data[i]);

        Serial.printf("\n");
    });

    for (int i=0; i<numInputPins; i++)
        dev.at(0x6400,1+i).set(inputPins[i].getValue());
}

void loop() {
	espBus.loop();
    digitalWrite(8,dev.at(0x2000,0).get<bool>());

    for (int i=0; i<numInputPins; i++) {
        if (inputPins[i].didChange()) {
            Serial.printf("change!!! %d\n",inputPins[i].getValue());
            dev.at(0x6400,1+i).set(inputPins[i].getValue());
        }
    }
}
