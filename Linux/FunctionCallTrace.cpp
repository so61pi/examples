#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

#include <cxxabi.h>
#include <dlfcn.h>


// need compiler flag -finstrument-functions -rdymanic -ldl

extern "C" {

void __attribute__((__no_instrument_function__)) __cyg_profile_func_enter(void *func, void *caller);
void __attribute__((__no_instrument_function__)) __cyg_profile_func_exit(void *func, void *caller);

}


static unsigned level;


// avoid infinite recursive calls
// __cyg_profile_func_enter -> Indent -> __cyg_profile_func_enter -> Indent...
void __attribute__((__no_instrument_function__)) Indent(unsigned level);

void Indent(unsigned level) {
    while (level) {
        printf("| ");
        --level;
    }
}


char* __attribute__((__no_instrument_function__)) AddressToDemangledName(void* address);

char* AddressToDemangledName(void* address) {
    Dl_info info = {};
    if (dladdr(address, &info) && info.dli_sname) {
        int status = 0;
        return abi::__cxa_demangle(info.dli_sname, 0, 0, &status);
    }
    return nullptr;
}


void __cyg_profile_func_enter(void* func, void* caller) {
    // this function can be called before main,
    // and if we call std::cout int this function,
    // it can lead to SIGSEGV

    char* funcName   = AddressToDemangledName(func);
    char* callerName = AddressToDemangledName(caller);

    Indent(level++);
    printf("enter %p [ %s ], caller %p [ %s ]\n", func, funcName ? funcName : "", caller, callerName ? callerName : "");

    free(funcName);
    free(callerName);
}


void __cyg_profile_func_exit(void* func, void* /*caller*/) {
    Indent(--level);
    printf("`leave %p\n", func);
}


void B() {
}


void A() {
    B();
}


int main() {
    A();
}

