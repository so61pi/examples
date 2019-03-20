#include <csignal>
#include <iostream>


void show(char ocnst* msg) {
    std::raise(SIGTRAP); // gdb will stop in the signal handler (like breakpoint).
                         // just use nexti to get out of there
    std::cout << msg << std::endl;
}


int main() {
    show("hello world");
}