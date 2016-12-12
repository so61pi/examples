#include <cstdio>
#include <cstdlib>

#include <cxxabi.h>
#include <dlfcn.h>

#include <unistd.h>
#include <sys/syscall.h>
#define gettid() syscall(SYS_gettid)


// need compiler flag -finstrument-functions -rdymanic -ldl


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


extern "C" {

void __attribute__((__no_instrument_function__)) __cyg_profile_func_enter(void *func, void *caller);
void __attribute__((__no_instrument_function__)) __cyg_profile_func_exit(void *func, void *caller);

}


static __thread unsigned level = 0;


// avoid infinite recursive calls
// __cyg_profile_func_enter -> Indent -> __cyg_profile_func_enter -> Indent...
void __attribute__((__no_instrument_function__)) Indent(unsigned level);

void Indent(unsigned level) {
    if (level == 0) {
        printf("%ld ", gettid());
        return;
    }

    unsigned i = 1;
    printf("%ld%s |", gettid(), Colors[i++ % 7]);
    while (i < level) {
        printf("%s |", Colors[i++ % 7]);
    }
    if (i == level) printf("%s |", Colors[i % 7]);
    printf("%s", NoColor);
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
