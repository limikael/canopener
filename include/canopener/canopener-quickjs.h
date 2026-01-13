#pragma once
extern "C" {
    #include "quickjs.h"
}
#include "canopener.h"
namespace canopener {
    void canopener_quickjs_init(JSContext *ctx);
    void canopener_quickjs_add_Bus(JSContext *ctx, const char *name, Bus* val);
    void canopener_quickjs_add_RemoteDevice(JSContext *ctx, const char *name, RemoteDevice* val);
    void canopener_quickjs_add_Entry(JSContext *ctx, const char *name, Entry* val);
    void canopener_quickjs_add_MasterDevice(JSContext *ctx, const char *name, MasterDevice* val);
}