#include <future>
#include <iostream>
#include <string>


int add(int a, int b) {
    return a + b;
}


int main() {
    // start add in new thread, and store return value to a future
    std::future<int> ft = std::async(std::launch::async, add, 1, 2);
    std::cout << ft.get() << std::endl;
}
