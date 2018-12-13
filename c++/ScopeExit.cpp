#include <atomic>
#include <iostream>
#include <boost/scope_exit.hpp>


//
// capture no variable
//
void Test() {
    std::cout << "Test()\n";

    BOOST_SCOPE_EXIT(void) {
        std::cout << "End scope.\n";
    } BOOST_SCOPE_EXIT_END;
    
    std::cout << "Inside Test().\n";
}


//
// use captured variable
//
void Test2() {
    std::cout << "Test2()\n";

    std::atomic<bool> is_committed = false;

    BOOST_SCOPE_EXIT(&is_committed) {
        if (is_committed)
            std::cout << "Committed.\n";
        else
            std::cout << "Rollback.\n";
    } BOOST_SCOPE_EXIT_END;

    std::cout << "Do something.\n";
}


//
// capture all variables, use lambda syntax
//
void Test3() {
    std::cout << "Test3()\n";

    std::atomic<bool> is_committed = false;

    BOOST_SCOPE_EXIT_ALL(&) {
        if (is_committed)
            std::cout << "Committed.\n";
        else
            std::cout << "Rollback.\n";
    };

    std::cout << "Do something.\n";
    is_committed = true;
}


int main() {
    Test();
    Test2();
    Test3();
}
