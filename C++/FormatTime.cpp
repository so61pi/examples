#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>


int main() {
    // get current time
    //
    // std::chrono::system_clock sc{};
    // auto current_time = sc.to_time_t(sc.now());
    auto current_time = std::time(nullptr);
    std::tm local_time{};
    localtime_s(&local_time, &current_time); // localtime is unsafe, according to MSVC

    // format
    std::vector<char> buffer(100);
    std::strftime(buffer.data(), buffer.size(), "%Y %m %d - %H:%M:%S", &local_time);

    // print it out
    std::cout << buffer.data() << "\n";

    // or simpler
    std::cout << std::put_time(&local_time, "%Y %m %d - %H:%M:%S") << "\n";
}
