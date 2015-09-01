#include <cassert>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <mutex>
#include <thread>


std::mutex mutex;
std::condition_variable cv;
std::exception_ptr eptr;
bool run_flag{ false };
bool exit_flag{ false };

int param;
int result;


int calc(int val) {
    if (eptr) {
        std::rethrow_exception(eptr);
    }

    std::unique_lock<std::mutex> lk{ mutex };
    param = val;
    run_flag = true;
    cv.notify_one();
    cv.wait(lk, []() -> bool { return !run_flag; });

    return result;
}


void exit() {
    if (eptr) {
        std::rethrow_exception(eptr);
    }

    std::unique_lock<std::mutex> lk{ mutex };
    exit_flag = true;
    cv.notify_one();
    cv.wait(lk, []() -> bool { return !exit_flag; });
}


void worker() {
    try {
        while (true) {
            std::unique_lock<std::mutex> lk{ mutex };
            cv.wait(lk, []() -> bool { return run_flag | exit_flag; });
            if (run_flag) {
                result = param / 2;
                run_flag = false;
                cv.notify_one();
            }
            else if (exit_flag) {
                exit_flag = false;
                cv.notify_one();
                return;
            }
            else {
                assert("cannot be here");
            }
        }
    }
    catch (...) {
        eptr = std::current_exception();
    }
}


int main() {
    try {
        std::thread w{ worker };
        std::cout << calc(2) << "\n";
        exit();
        w.join();
    }
    catch (std::exception const& e) {
        std::cout << "exception : " << e.what() << "\n";
    }
}
