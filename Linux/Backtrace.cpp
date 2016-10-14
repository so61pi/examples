#include <cstdlib>
#include <iostream>

#include <execinfo.h>


// need compiler flag -rdynamic to have meaningful output


void show() {
    int const BUFSIZE = 100;
    void* buffer[BUFSIZE] = {nullptr};
    auto size = backtrace(buffer, BUFSIZE);
    auto strings = backtrace_symbols(buffer, size);
    if (strings == nullptr) return;
    for (auto i = 0; i < size; ++i) {
        std::cout << strings[i] << std::endl;
    }
    free(strings);
}


int main() {
    show();
}
