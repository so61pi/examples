#include <cassert>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <stdexcept>

#include <boost/scope_exit.hpp>


void task(std::shared_ptr<bool> pRun) {
    assert(pRun);
    while (*pRun) {
    }
}


int main() {
    try {
        auto const pRun = std::make_shared<bool>(true);

        // std::future destructor will wait until the task completes.
        auto ft = std::async(std::launch::async, task, pRun);

        // This must be after the constructing of ft to make sure that
        // it will run before ft's destructor.
        BOOST_SCOPE_EXIT_ALL(&) { *pRun = false; };

        throw std::runtime_error("exception");

        *pRun = false;
        ft.get();
    } catch (std::exception const& e) {
        std::cout << e.what() << std::endl;
    }
}
