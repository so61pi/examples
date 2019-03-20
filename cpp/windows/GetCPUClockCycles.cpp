#include <iostream>
#include <Windows.h>
#include <intrin.h>


int main() {
    //
    // Read more about the problem of rdtsc instruction
    // (run out-of-oder & multiple-core):
    //    http://stackoverflow.com/a/13772803/2064646
    //
    std::cout << "CPU clock cycles: " << __rdtsc() << "\n";

    LARGE_INTEGER perf_counter{};
    QueryPerformanceCounter(&perf_counter);
    std::cout << "Performance counter: " << perf_counter.QuadPart << "\n";
}
