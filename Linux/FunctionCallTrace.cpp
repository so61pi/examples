#include <cstdio>
#include <iostream>
#include <string>

// need compiler flag -finstrument-functions

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


void __cyg_profile_func_enter(void* func, void* caller) {
    // this function can be called before main,
    // and if we call std::cout int this function,
    // it can lead to SIGSEGV
    Indent(level++);
    printf("enter %p, caller %p\n", func, caller);
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
