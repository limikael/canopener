#include <cassert>
#include "bindings.out.h"

JSVAL jsvalEvalChecked(std::string code) {
    JSVAL res=jsvalEval(code.c_str());
    if (jsvalHasException()) {
        std::string ex=jsvalCatchExceptionStdString();
        printf("Eval Error: %s\n",ex.c_str());
        abort();
    }

    return res;
}

void test_bindings() {
    jsvalQuickjsInit();
    canopener_init_jsval();

    JSVAL v=jsvalEval("[1,2,3]");
    std::string vs=jsvalToStdString(v);
    jsvalFree(v);
    assert(vs=="1,2,3");

    canopener_exit();
    jsvalQuickjsExit();
}