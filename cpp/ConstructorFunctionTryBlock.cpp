#include <exception>
#include <iostream>
#include <stdexcept>


class Base {
public:
    Base() {
        throw std::runtime_error{ "Base::Base exception" };
    }
};


class Derived : public Base
{
public:
    // function try block constructor
    // used to catch
    //     exception when initializing subobject (base classes, member data)
    //     exception in the try body
    //
    // read more
    //     http://en.cppreference.com/w/cpp/language/function-try-block
    //     http://www.gotw.ca/gotw/066.htm
    Derived()
    try
        : Base{}
    {
        throw std::runtime_error{ "Derived::Derived exception" };
    }
    catch (std::exception const& e) {
        std::cout << "Derived handler : " << e.what() << "\n";
    } // rethrow automatically
    catch (...) {
        std::cout << "Derived handler : unknown exception\n";
    } // rethrow automatically
};


int main() {
    try {
        Derived derived;
    }
    catch (std::exception const& e) {
        std::cout << "main handler : " << e.what() << "\n";
    }
    catch (...) {
        std::cout << "main handler : unknown exception\n";
    }
}
