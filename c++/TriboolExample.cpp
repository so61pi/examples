#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <boost/logic/tribool.hpp>


boost::tribool g_status{ boost::indeterminate };
std::mutex g_statusMutex{};


//
// working thread
//
void WorkThread() {
    std::chrono::system_clock c{};

    // working...
    std::this_thread::sleep_for(std::chrono::seconds{ 5 });

    // ... and set the return state (randomly)
    std::lock_guard<std::mutex> lg{ g_statusMutex };
    g_status = (c.to_time_t(c.now()) % 2 == 0);
}


int main() {
    // start working thread
    std::thread t(WorkThread);

    while (true) {
        // sleep
        std::this_thread::sleep_for(std::chrono::milliseconds{ 500 });

        //
        // get job status
        // can use indeterminate function instead
        //
        // if (boost::indeterminate(g_status)) {
        //     // indeterminate status
        //     std::cout << "Still working.\n";
        // }
        //
        std::lock_guard<std::mutex> lg{ g_statusMutex };
        if (g_status) {
            // true state
            std::cout << "Job done successfully.\n";
            break;
        }
        else if (!g_status) {
            // false state
            std::cout << "Job done unsuccessfully.\n";
            break;
        }
        else {
            // indeterminate state
            std::cout << "Still working.\n";
        }
    }

    // wait for working thread
    t.join();
}
