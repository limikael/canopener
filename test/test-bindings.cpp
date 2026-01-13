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
    JSRuntime *rt=JS_NewRuntime();
    JSContext *ctx=JS_NewContext(rt);
    BusHub hub;
    Bus* masterBus=hub.createBus();
    Bus* deviceBus=hub.createBus();
    Bus* observerBus=hub.createBus();

	printf("- bindings\n");

    canopener_quickjs_init(ctx);
    canopener_quickjs_add_Bus(ctx,"canBus",masterBus);

	std::string s=runjs(ctx,"\
		let master=new MasterDevice(canBus); \
        let device=master.createRemoteDevice(5); \
        device.insert(0x4000,0).set(0x12345678); \
        let v=device.at(0x4000,0).get(); \
        [v] \
	");

    //std::cout << std::format("{}\n",s);
    assert(s=="305419896");

    JS_FreeContext(ctx);
    JS_FreeRuntime(rt);
}