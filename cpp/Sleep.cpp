#include <chrono>
#include <thread>


int main() {
    // make this thread go to sleep for 2 seconds
    std::this_thread::sleep_for(std::chrono::seconds{ 2 });

    // sleep until next 5 seconds
    std::this_thread::sleep_until(
        std::chrono::system_clock::now() + std::chrono::seconds{ 5 });
}
