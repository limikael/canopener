#include <cstdio>
#include <cassert>
#include <string>
#include <format>
#include <iostream>

extern "C" {
#include "quickjs.h"
}

#include "canopener.h"

#include "canopener/canopener-quickjs.h"

using namespace canopener;

std::string runjs(JSContext *ctx, const char *code) {
    JSValue result=JS_Eval(ctx,
        code,
        strlen(code),
        "<input>",
        JS_EVAL_TYPE_GLOBAL
    );

    if (JS_IsException(result)) {
        JSValue ex=JS_GetException(ctx);
        const char *msg=JS_ToCString(ctx,ex);
        printf("exception: %s\n",msg);
        JS_FreeCString(ctx,msg);
        JS_FreeValue(ctx,ex);
    }

    assert(!JS_IsException(result));

    const char *res=JS_ToCString(ctx,result);
    std::string resString=std::string(res);
    JS_FreeCString(ctx,res);
    JS_FreeValue(ctx,result);

    return resString;
}

void test_bindings() {
    printf("- bindings\n");

    JSRuntime *rt=JS_NewRuntime();
    JSContext *ctx=JS_NewContext(rt);
    MockBus bus;

    Device dev(bus);
    dev.setNodeId(5);
    dev.insert(0x4000,0);
    dev.insert(0x4000,1);

    canopener_quickjs_init(ctx);
    canopener_quickjs_add_Bus(ctx,"canBus",&bus);

	std::string s=runjs(ctx,"\
		let master=new MasterDevice(canBus); \
        let device=master.createRemoteDevice(5); \
        device.insert(0x4000,0).set(0x12345678); \
        device.insert(0x4000,1).set(0x11111111); \
        let v=device.at(0x4000,0).get(); \
        [v] \
	");

    for (int i=0; i<10; i++)
        bus.loop();

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    /*for (auto it: bus.log)
        std::cout << std::format("{}\n",it);*/

    assert(bus.log.size()==4);
}