#ifdef CANOPENER_QUICKJS
extern "C" {
    #include "quickjs.h"
}
#include <string>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include "canopener.h"
using namespace canopener;
namespace canopener {
    typedef struct {
        void *instance;
        bool owned;
    } canopener_quickjs_opaque_t;
    typedef struct {
        Dispatcher<> *dispatcher;
        int id;
    } canopener_quickjs_listener_t;
    std::vector<canopener_quickjs_listener_t *> canopener_quickjs_listeners;
    canopener_quickjs_opaque_t* canopener_quickjs_opaque_create(void *instance, bool owned) {
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)malloc(sizeof(canopener_quickjs_opaque_t));
        opaque->instance=instance;
        opaque->owned=owned;
        return opaque;
    }
    static JSClassID canopener_quickjs_Bus_classid=0;
    static JSClassID canopener_quickjs_RemoteDevice_classid=0;
    static JSClassID canopener_quickjs_Entry_classid=0;
    static JSClassID canopener_quickjs_MasterDevice_classid=0;
    static JSValue canopener_quickjs_Bus_ctor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        return JS_ThrowTypeError(ctx, "abstract");
    }
    static void canopener_quickjs_Bus_finalizer(JSRuntime *rt, JSValue obj) {
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(obj,canopener_quickjs_Bus_classid);
        if (opaque->owned) {
            Bus* instance=(Bus*)opaque->instance;
            delete instance;
        }
        free(opaque);
    }
    static JSValue canopener_quickjs_RemoteDevice_ctor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        return JS_ThrowTypeError(ctx, "abstract");
    }
    static void canopener_quickjs_RemoteDevice_finalizer(JSRuntime *rt, JSValue obj) {
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(obj,canopener_quickjs_RemoteDevice_classid);
        if (opaque->owned) {
            RemoteDevice* instance=(RemoteDevice*)opaque->instance;
            delete instance;
        }
        free(opaque);
    }
    static JSValue canopener_quickjs_RemoteDevice_insert(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=2) return JS_ThrowTypeError(ctx, "wrong arg count");
        int32_t arg_0;
        int32_t arg_1;
        JS_ToInt32(ctx,&arg_0,argv[0]);
        JS_ToInt32(ctx,&arg_1,argv[1]);
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        //RemoteDevice* instance=(RemoteDevice*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        Entry* ret;
        ret=&instance->insert(arg_0,arg_1);
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_Entry_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    static JSValue canopener_quickjs_RemoteDevice_at(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=2) return JS_ThrowTypeError(ctx, "wrong arg count");
        int32_t arg_0;
        int32_t arg_1;
        JS_ToInt32(ctx,&arg_0,argv[0]);
        JS_ToInt32(ctx,&arg_1,argv[1]);
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        //RemoteDevice* instance=(RemoteDevice*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        Entry* ret;
        ret=&instance->at(arg_0,arg_1);
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_Entry_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    static JSValue canopener_quickjs_RemoteDevice_getNodeId(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        //RemoteDevice* instance=(RemoteDevice*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        int32_t ret;
        ret=instance->getNodeId();
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewInt32(ctx,ret);
        return retval;
    }
    static JSValue canopener_quickjs_RemoteDevice_getGeneration(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        //RemoteDevice* instance=(RemoteDevice*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        int32_t ret;
        ret=instance->getGeneration();
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewInt32(ctx,ret);
        return retval;
    }
    static JSValue canopener_quickjs_RemoteDevice_getCommitGeneration(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        //RemoteDevice* instance=(RemoteDevice*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        int32_t ret;
        ret=instance->getCommitGeneration();
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewInt32(ctx,ret);
        return retval;
    }
    static JSValue canopener_quickjs_RemoteDevice_isRefreshInProgress(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        //RemoteDevice* instance=(RemoteDevice*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        int32_t ret;
        ret=instance->isRefreshInProgress();
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewInt32(ctx,ret);
        return retval;
    }
    static JSValue canopener_quickjs_RemoteDevice_on(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=2) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        const char *eventName=JS_ToCString(ctx,argv[0]);
        Dispatcher<> *dispatcher;
        if (!strcmp(eventName,"commitGenerationChange")) dispatcher=&instance->commitGenerationChangeDispatcher;
        JS_FreeCString(ctx,eventName);
        if (!dispatcher) return JS_ThrowTypeError(ctx, "unknown event");
        void* identity=JS_VALUE_GET_PTR(argv[1]);
        if (dispatcher->getIdByIdentity(identity)) return JS_UNDEFINED; // existing
        JSValue fnDup=JS_DupValue(ctx,argv[1]);
        int id=dispatcher->on([ctx, fnDup]() {
            JSValue ret=JS_Call(ctx, fnDup, JS_UNDEFINED, 0, NULL);
            JS_FreeValue(ctx,ret);
        });
        canopener_quickjs_listener_t *listener=new canopener_quickjs_listener_t {
            .dispatcher=dispatcher,
            .id=id
        };
        canopener_quickjs_listeners.push_back(listener);
        dispatcher->setIdentity(id,identity);
        dispatcher->setDestructor(id,[ctx, fnDup, listener]() {
            canopener_quickjs_listeners.erase(std::remove(canopener_quickjs_listeners.begin(), canopener_quickjs_listeners.end(), listener), canopener_quickjs_listeners.end());
            delete listener;
            JS_FreeValue(ctx,fnDup);
        });
        return JS_UNDEFINED;
    }
    static JSValue canopener_quickjs_RemoteDevice_off(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc>2 || argc<1) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_RemoteDevice_classid);
        RemoteDevice* instance=(RemoteDevice*)opaque->instance;
        const char *eventName=JS_ToCString(ctx,argv[0]);
        Dispatcher<> *dispatcher=nullptr;
        if (!strcmp(eventName,"commitGenerationChange")) dispatcher=&instance->commitGenerationChangeDispatcher;
        JS_FreeCString(ctx,eventName);
        if (!dispatcher) return JS_ThrowTypeError(ctx, "unknown event");
        if (argc==1) {
            dispatcher->off();
            return JS_UNDEFINED;
        }
        JSValueConst fn = argv[1];
        void* identity = JS_VALUE_GET_PTR(fn);
        int id=dispatcher->getIdByIdentity(identity);
        if (id) dispatcher->off(id);
        return JS_UNDEFINED;
    }
    static JSValue canopener_quickjs_Entry_ctor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        return JS_ThrowTypeError(ctx, "abstract");
    }
    static void canopener_quickjs_Entry_finalizer(JSRuntime *rt, JSValue obj) {
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(obj,canopener_quickjs_Entry_classid);
        if (opaque->owned) {
            Entry* instance=(Entry*)opaque->instance;
            delete instance;
        }
        free(opaque);
    }
    static JSValue canopener_quickjs_Entry_set(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=1) return JS_ThrowTypeError(ctx, "wrong arg count");
        int32_t arg_0;
        JS_ToInt32(ctx,&arg_0,argv[0]);
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        //Entry* instance=(Entry*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        Entry* instance=(Entry*)opaque->instance;
        Entry* ret;
        ret=&instance->set(arg_0);
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_Entry_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    static JSValue canopener_quickjs_Entry_get(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        //Entry* instance=(Entry*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        Entry* instance=(Entry*)opaque->instance;
        int32_t ret;
        ret=instance->get<int>();
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewInt32(ctx,ret);
        return retval;
    }
    static JSValue canopener_quickjs_Entry_subscribe(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=1) return JS_ThrowTypeError(ctx, "wrong arg count");
        int32_t arg_0;
        JS_ToInt32(ctx,&arg_0,argv[0]);
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        //Entry* instance=(Entry*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        Entry* instance=(Entry*)opaque->instance;
        Entry* ret;
        ret=&instance->subscribe(arg_0);
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_Entry_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    static JSValue canopener_quickjs_Entry_refresh(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=0) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        //Entry* instance=(Entry*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        Entry* instance=(Entry*)opaque->instance;
        Entry* ret;
        ret=&instance->refresh();
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_Entry_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    static JSValue canopener_quickjs_Entry_on(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=2) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        Entry* instance=(Entry*)opaque->instance;
        const char *eventName=JS_ToCString(ctx,argv[0]);
        Dispatcher<> *dispatcher;
        if (!strcmp(eventName,"change")) dispatcher=&instance->changeDispatcher;
        JS_FreeCString(ctx,eventName);
        if (!dispatcher) return JS_ThrowTypeError(ctx, "unknown event");
        void* identity=JS_VALUE_GET_PTR(argv[1]);
        if (dispatcher->getIdByIdentity(identity)) return JS_UNDEFINED; // existing
        JSValue fnDup=JS_DupValue(ctx,argv[1]);
        int id=dispatcher->on([ctx, fnDup]() {
            JSValue ret=JS_Call(ctx, fnDup, JS_UNDEFINED, 0, NULL);
            JS_FreeValue(ctx,ret);
        });
        canopener_quickjs_listener_t *listener=new canopener_quickjs_listener_t {
            .dispatcher=dispatcher,
            .id=id
        };
        canopener_quickjs_listeners.push_back(listener);
        dispatcher->setIdentity(id,identity);
        dispatcher->setDestructor(id,[ctx, fnDup, listener]() {
            canopener_quickjs_listeners.erase(std::remove(canopener_quickjs_listeners.begin(), canopener_quickjs_listeners.end(), listener), canopener_quickjs_listeners.end());
            delete listener;
            JS_FreeValue(ctx,fnDup);
        });
        return JS_UNDEFINED;
    }
    static JSValue canopener_quickjs_Entry_off(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc>2 || argc<1) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_Entry_classid);
        Entry* instance=(Entry*)opaque->instance;
        const char *eventName=JS_ToCString(ctx,argv[0]);
        Dispatcher<> *dispatcher=nullptr;
        if (!strcmp(eventName,"change")) dispatcher=&instance->changeDispatcher;
        JS_FreeCString(ctx,eventName);
        if (!dispatcher) return JS_ThrowTypeError(ctx, "unknown event");
        if (argc==1) {
            dispatcher->off();
            return JS_UNDEFINED;
        }
        JSValueConst fn = argv[1];
        void* identity = JS_VALUE_GET_PTR(fn);
        int id=dispatcher->getIdByIdentity(identity);
        if (id) dispatcher->off(id);
        return JS_UNDEFINED;
    }
    static JSValue canopener_quickjs_MasterDevice_ctor(JSContext *ctx, JSValueConst new_target, int argc, JSValueConst *argv) {
        if (argc!=1) return JS_ThrowTypeError(ctx, "wrong arg count");
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(argv[0],canopener_quickjs_Bus_classid);
        Bus& arg_0=*(Bus*)opaque->instance;
        MasterDevice* instance=new MasterDevice(arg_0);
        JSValue obj=JS_NewObjectClass(ctx,canopener_quickjs_MasterDevice_classid);
        JS_SetOpaque(obj,canopener_quickjs_opaque_create(instance,true));
        return obj;
    }
    static void canopener_quickjs_MasterDevice_finalizer(JSRuntime *rt, JSValue obj) {
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(obj,canopener_quickjs_MasterDevice_classid);
        if (opaque->owned) {
            MasterDevice* instance=(MasterDevice*)opaque->instance;
            delete instance;
        }
        free(opaque);
    }
    static JSValue canopener_quickjs_MasterDevice_createRemoteDevice(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=1) return JS_ThrowTypeError(ctx, "wrong arg count");
        int32_t arg_0;
        JS_ToInt32(ctx,&arg_0,argv[0]);
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_MasterDevice_classid);
        //MasterDevice* instance=(MasterDevice*)JS_GetOpaque(thisobj,canopener_quickjs_MasterDevice_classid);
        MasterDevice* instance=(MasterDevice*)opaque->instance;
        RemoteDevice* ret;
        ret=instance->createRemoteDevice(arg_0);
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_RemoteDevice_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    static JSValue canopener_quickjs_MasterDevice_getRemoteDevice(JSContext *ctx, JSValueConst thisobj, int argc, JSValueConst *argv) {
        if (argc!=1) return JS_ThrowTypeError(ctx, "wrong arg count");
        int32_t arg_0;
        JS_ToInt32(ctx,&arg_0,argv[0]);
        canopener_quickjs_opaque_t* opaque=(canopener_quickjs_opaque_t*)JS_GetOpaque(thisobj,canopener_quickjs_MasterDevice_classid);
        //MasterDevice* instance=(MasterDevice*)JS_GetOpaque(thisobj,canopener_quickjs_MasterDevice_classid);
        MasterDevice* instance=(MasterDevice*)opaque->instance;
        RemoteDevice* ret;
        ret=instance->getRemoteDevice(arg_0);
        JSValue retval=JS_UNDEFINED;
        retval=JS_NewObjectClass(ctx,canopener_quickjs_RemoteDevice_classid);
        JS_SetOpaque(retval,canopener_quickjs_opaque_create(ret,false));
        return retval;
    }
    void canopener_quickjs_init(JSContext *ctx) {
        JSValue global=JS_GetGlobalObject(ctx);
        if (!canopener_quickjs_Bus_classid) JS_NewClassID(&canopener_quickjs_Bus_classid);
        JSClassDef Bus_def={.class_name="Bus", .finalizer=canopener_quickjs_Bus_finalizer};
        JS_NewClass(JS_GetRuntime(ctx),canopener_quickjs_Bus_classid,&Bus_def);
        JSValue Bus_proto=JS_NewObject(ctx);
        JS_SetClassProto(ctx, canopener_quickjs_Bus_classid,Bus_proto);
        JSValue Bus_ctorval=JS_NewCFunction2(ctx,canopener_quickjs_Bus_ctor,"Bus",0,JS_CFUNC_constructor,0);
        JS_SetConstructor(ctx,Bus_ctorval,Bus_proto);
        JS_SetPropertyStr(ctx,global,"Bus",Bus_ctorval);
        if (!canopener_quickjs_RemoteDevice_classid) JS_NewClassID(&canopener_quickjs_RemoteDevice_classid);
        JSClassDef RemoteDevice_def={.class_name="RemoteDevice", .finalizer=canopener_quickjs_RemoteDevice_finalizer};
        JS_NewClass(JS_GetRuntime(ctx),canopener_quickjs_RemoteDevice_classid,&RemoteDevice_def);
        JSValue RemoteDevice_proto=JS_NewObject(ctx);
        JS_SetClassProto(ctx, canopener_quickjs_RemoteDevice_classid,RemoteDevice_proto);
        JSValue RemoteDevice_ctorval=JS_NewCFunction2(ctx,canopener_quickjs_RemoteDevice_ctor,"RemoteDevice",0,JS_CFUNC_constructor,0);
        JS_SetConstructor(ctx,RemoteDevice_ctorval,RemoteDevice_proto);
        JS_SetPropertyStr(ctx,global,"RemoteDevice",RemoteDevice_ctorval);
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"insert",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_insert,"insert",0));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"at",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_at,"at",0));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"getNodeId",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_getNodeId,"getNodeId",0));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"getGeneration",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_getGeneration,"getGeneration",0));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"getCommitGeneration",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_getCommitGeneration,"getCommitGeneration",0));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"isRefreshInProgress",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_isRefreshInProgress,"isRefreshInProgress",0));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"on",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_on,"on",2));
        JS_SetPropertyStr(ctx,RemoteDevice_proto,"off",JS_NewCFunction(ctx, canopener_quickjs_RemoteDevice_off,"off",2));
        if (!canopener_quickjs_Entry_classid) JS_NewClassID(&canopener_quickjs_Entry_classid);
        JSClassDef Entry_def={.class_name="Entry", .finalizer=canopener_quickjs_Entry_finalizer};
        JS_NewClass(JS_GetRuntime(ctx),canopener_quickjs_Entry_classid,&Entry_def);
        JSValue Entry_proto=JS_NewObject(ctx);
        JS_SetClassProto(ctx, canopener_quickjs_Entry_classid,Entry_proto);
        JSValue Entry_ctorval=JS_NewCFunction2(ctx,canopener_quickjs_Entry_ctor,"Entry",0,JS_CFUNC_constructor,0);
        JS_SetConstructor(ctx,Entry_ctorval,Entry_proto);
        JS_SetPropertyStr(ctx,global,"Entry",Entry_ctorval);
        JS_SetPropertyStr(ctx,Entry_proto,"set",JS_NewCFunction(ctx, canopener_quickjs_Entry_set,"set",0));
        JS_SetPropertyStr(ctx,Entry_proto,"get",JS_NewCFunction(ctx, canopener_quickjs_Entry_get,"get",0));
        JS_SetPropertyStr(ctx,Entry_proto,"subscribe",JS_NewCFunction(ctx, canopener_quickjs_Entry_subscribe,"subscribe",0));
        JS_SetPropertyStr(ctx,Entry_proto,"refresh",JS_NewCFunction(ctx, canopener_quickjs_Entry_refresh,"refresh",0));
        JS_SetPropertyStr(ctx,Entry_proto,"on",JS_NewCFunction(ctx, canopener_quickjs_Entry_on,"on",2));
        JS_SetPropertyStr(ctx,Entry_proto,"off",JS_NewCFunction(ctx, canopener_quickjs_Entry_off,"off",2));
        if (!canopener_quickjs_MasterDevice_classid) JS_NewClassID(&canopener_quickjs_MasterDevice_classid);
        JSClassDef MasterDevice_def={.class_name="MasterDevice", .finalizer=canopener_quickjs_MasterDevice_finalizer};
        JS_NewClass(JS_GetRuntime(ctx),canopener_quickjs_MasterDevice_classid,&MasterDevice_def);
        JSValue MasterDevice_proto=JS_NewObject(ctx);
        JS_SetClassProto(ctx, canopener_quickjs_MasterDevice_classid,MasterDevice_proto);
        JSValue MasterDevice_ctorval=JS_NewCFunction2(ctx,canopener_quickjs_MasterDevice_ctor,"MasterDevice",0,JS_CFUNC_constructor,0);
        JS_SetConstructor(ctx,MasterDevice_ctorval,MasterDevice_proto);
        JS_SetPropertyStr(ctx,global,"MasterDevice",MasterDevice_ctorval);
        JS_SetPropertyStr(ctx,MasterDevice_proto,"createRemoteDevice",JS_NewCFunction(ctx, canopener_quickjs_MasterDevice_createRemoteDevice,"createRemoteDevice",0));
        JS_SetPropertyStr(ctx,MasterDevice_proto,"getRemoteDevice",JS_NewCFunction(ctx, canopener_quickjs_MasterDevice_getRemoteDevice,"getRemoteDevice",0));
        JS_FreeValue(ctx,global);
    }
    void canopener_quickjs_exit(JSContext *ctx) {
        //printf("exiting, listeners=%d\n",canopener_quickjs_listeners.size());
        while (canopener_quickjs_listeners.size())
        canopener_quickjs_listeners[0]->dispatcher->off(canopener_quickjs_listeners[0]->id);
        //printf("exiting, listeners=%d\n",canopener_quickjs_listeners.size());
    }
    void canopener_quickjs_add_Bus(JSContext *ctx, const char *name, Bus* val) {
        JSValue global=JS_GetGlobalObject(ctx);
        JSValue v=JS_NewObjectClass(ctx,canopener_quickjs_Bus_classid);
        JS_SetOpaque(v,canopener_quickjs_opaque_create(val,false));
        JS_SetPropertyStr(ctx,global,name,v);
        JS_FreeValue(ctx,global);
    }
    void canopener_quickjs_add_RemoteDevice(JSContext *ctx, const char *name, RemoteDevice* val) {
        JSValue global=JS_GetGlobalObject(ctx);
        JSValue v=JS_NewObjectClass(ctx,canopener_quickjs_RemoteDevice_classid);
        JS_SetOpaque(v,canopener_quickjs_opaque_create(val,false));
        JS_SetPropertyStr(ctx,global,name,v);
        JS_FreeValue(ctx,global);
    }
    void canopener_quickjs_add_Entry(JSContext *ctx, const char *name, Entry* val) {
        JSValue global=JS_GetGlobalObject(ctx);
        JSValue v=JS_NewObjectClass(ctx,canopener_quickjs_Entry_classid);
        JS_SetOpaque(v,canopener_quickjs_opaque_create(val,false));
        JS_SetPropertyStr(ctx,global,name,v);
        JS_FreeValue(ctx,global);
    }
    void canopener_quickjs_add_MasterDevice(JSContext *ctx, const char *name, MasterDevice* val) {
        JSValue global=JS_GetGlobalObject(ctx);
        JSValue v=JS_NewObjectClass(ctx,canopener_quickjs_MasterDevice_classid);
        JS_SetOpaque(v,canopener_quickjs_opaque_create(val,false));
        JS_SetPropertyStr(ctx,global,name,v);
        JS_FreeValue(ctx,global);
    }
}
#endif // CANOPENER_QUICKJS