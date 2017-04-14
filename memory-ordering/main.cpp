#include <atomic>
#include <iostream>
#include <thread>


//
// to make this program work correctly, uncomment the line below
// #define USE_ATOMIC
#ifdef USE_ATOMIC
std::atomic<int> a{ 0 };
std::atomic<int> b{ 0 };
#else
volatile int a = 0;
volatile int b = 0;
#endif
volatile int r1 = 0;
volatile int r2 = 0;


//
std::atomic<bool> s1{ true };
std::atomic<bool> s2{ true };


void T1() {
    while(true) {
        // signal the main thread that the last job has been done
        s1.store(false);

        // wait for the signal from the main thread
        while (!s1.load());

        //
        a = 1;
        r1 = b;
    }
}


void T2() {
    while (true) {
        // signal the main thread that the last job has been done
        s2.store(false);

        // wait for the signal from the main thread
        while (!s2.load());

        //
        b = 1;
        r2 = a;
    }
}


int main() {
    // start 2 threads...
    std::thread t1(T1);
    std::thread t2(T2);

    // ... and wait for them to start
    while (s1.load() || s2.load());

    for (int i = 0;; ++i) {
        // reset value of a and b...
        a = b = 0;
        r1 = r2 = 1;
        std::atomic_thread_fence(std::memory_order_seq_cst);

        // ... signal the threads...
        s1.store(true);
        s2.store(true);

        // ... then wait for them to get their job done...
        while (s1.load() || s2.load());
        
        // ... and we will get surprising result
        if (r1 == 0 && r2 == 0)
            std::cout << i << " : " << "r1 = r2 = 0" << std::endl;
    }
    t1.join();
    t2.join();
}
