#include <future>
#include <iostream>
#include <string>


int main() {
    // create a packaged task with a lambda
    std::packaged_task<int(int, int)> task(
        [](int a, int b) -> int {
            return a + b;
        }
    );

    // get future to get returned value of task later
    std::future<int> ft = task.get_future();

    // execute the stored task asynchronously
    task(1, 2);

    // get returned value
    std::cout << ft.get() << std::endl;

    // task must be reset and future must be re-get if you want to run it again
    task.reset();
    ft = task.get_future();
    task(3, 4);
    std::cout << ft.get() << std::endl;
}
