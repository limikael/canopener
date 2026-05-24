#include <cassert>
#include "bindings.out.h"
#include "canopener/MockBus.h"
#include <memory>
#include <iostream>

using namespace canopener;

JSVAL jsvalEvalChecked(std::string code) {
    JSVAL res=jsvalEval(code.c_str());
    if (jsvalHasException()) {
        std::string ex=jsvalCatchExceptionStdString();
        printf("Eval Error: %s\n",ex.c_str());
        abort();
    }

    return res;
}

std::shared_ptr<MockBus> testBus;
std::shared_ptr<Bus> getBus() {
    return testBus;
}

void test_bindings() {
    jsvalQuickjsInit();
    canopener_init_jsval();

    testBus=std::make_shared<MockBus>();

    jsvalEvalChecked("globalThis.m=new MasterDevice(getBus()); undefined");
    jsvalEvalChecked("globalThis.counter=0");
    jsvalEvalChecked("globalThis.m.on('devicesChanged',()=>globalThis.counter++)");
    jsvalEvalChecked("globalThis.d=new Device(getBus()); globalThis.d.setNodeId(55); undefined");

    for (int i=0; i<10; i++) {
        testBus->mockMillis+=1000;
        testBus->loop();
    }

    JSVAL sz=jsvalEvalChecked("globalThis.m.getConnectedDeviceIds().size()");
    assert(jsvalGetInt(sz)==1);

    assert(jsvalGetInt(jsvalEvalChecked("globalThis.m.getConnectedDeviceIds().at(0)"))==55);
    jsvalEvalChecked("globalThis.d=null");

    for (int i=0; i<10; i++) {
        testBus->mockMillis+=1000;
        testBus->loop();
    }

    sz=jsvalEvalChecked("globalThis.m.getConnectedDeviceIds().size()");
    assert(jsvalGetInt(sz)==0);

    JSVAL v=jsvalEvalChecked("globalThis.counter");
    std::string s=jsvalToStdString(v);
    //printf("count: %s\n",s.c_str());
    assert(s=="2");

    /*for (int i=0; i<testBus->log.size(); i++)
        std::cout << testBus->log[i] << std::endl;*/

    canopener_exit();
    jsvalQuickjsExit();
}