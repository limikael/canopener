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

    canopener_quickjs_exit(ctx);

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    /*for (auto it: bus.log)
        std::cout << std::format("{}\n",it);*/

    assert(bus.log.size()==4);

    assert(dev.at(0x4000,0).get<uint32_t>()==0x12345678);
    assert(dev.at(0x4000,1).get<uint32_t>()==0x11111111);
}

void test_bindings_listeners() {
    printf("- bindings with listeners\n");

    JSRuntime *rt=JS_NewRuntime();
    JSContext *ctx=JS_NewContext(rt);
    MockBus bus;

    Device dev(bus);
    dev.setNodeId(5);
    dev.insert(0x4000,0);

    canopener_quickjs_init(ctx);
    canopener_quickjs_add_Bus(ctx,"canBus",&bus);

    std::string s=runjs(ctx,"\
        var v; \
        v=123; \
        let master=new MasterDevice(canBus); \
        let device=master.createRemoteDevice(5); \
        device.insert(0x4000,0).subscribe(1); \
        device.at(0x4000,0).on(\"change\",()=>{ v=555; }); \
        [v] \
    ");

    for (int i=0; i<10; i++)
        bus.loop();

    dev.at(0x4000,0).set<uint32_t>(777);

    for (int i=0; i<10; i++)
        bus.loop();

    std::string t=runjs(ctx,"\
        [v] \
    ");

    assert(t=="555");

    //printf("t after: %s\n",t.c_str());

    canopener_quickjs_exit(ctx);

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);

    /*for (auto it: bus.log)
        std::cout << std::format("{}\n",it);*/
}
