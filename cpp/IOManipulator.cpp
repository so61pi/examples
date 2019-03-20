#include <atomic>
#include <ios>
#include <iostream>


namespace myspace {
    int index{ 0 };
    std::atomic<bool> is_initialized{ false };


    struct integer {
        int value;
    };


    std::ios_base& setmanip(std::ios_base& ios) {
        if (!is_initialized) {
            index = ios.xalloc();
            ios.iword(index) = 1;
            is_initialized = true;
        }
        else {
            ios.iword(index) = 1;
        }
        return ios;
    }


    std::ios_base& clrmanip(std::ios_base& ios) {
        if (is_initialized) {
            ios.iword(index) = 0;
        }
        return ios;
    }


    std::ostream& operator<<(std::ostream& os, integer const& num) {
        if (is_initialized && os.iword(index) == 1) {
            os << num.value * 2;
        }
        else {
            os << num.value;
        }
        return os;
    }
} // namespace myspace


int main() {
    myspace::integer num{ 10 };
    std::cout
        << myspace::setmanip << num << "\n"
        << myspace::clrmanip << num << "\n";
}
