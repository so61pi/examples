#include <cstdio>
#include <cstdlib>

#include <cxxabi.h>
#include <dlfcn.h>


#define HAVE_COLOR
#ifdef HAVE_COLOR
    char const* const Colors[] = {
        "\033[96m",
        "\033[91m",
        "\033[92m",
        "\033[95m",
        "\033[93m",
        "\033[94m",
        "\033[97m"
    };
    char const* const NoColor = "\033[0m";
#else
    char const* const Colors[] = {
        "",
        "",
        "",
        "",
        "",
        "",
        ""
    };
    char const* const NoColor = "";
#endif


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
    if (level == 0) return;

    for (unsigned i = 1; i < level; ++i) {
        printf("%s| ", Colors[i % 7]);
    }
    printf("%s|%s", Colors[level % 7], NoColor);
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
    printf("-enter [ %s ] from [ %s ] (enter %p from %p) \n", funcName ? funcName : "", callerName ? callerName : "", func, caller);

    free(funcName);
    free(callerName);
}


void __cyg_profile_func_exit(void* /*func*/, void* /*caller*/) {
    Indent(--level);
    if (level) printf(" `---\n");
    else printf("`---\n");
}


void B(int counter) {
    if (counter) B(--counter);
}


void A(int counter) {
    B(counter);
}


int main() {
    for (unsigned i = 0; i < 10; ++i) {
        A(i);
    }
}
