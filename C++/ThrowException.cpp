#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>


class demo {
public:
    demo() {
        std::cout << "demo()\n";
    }


    ~demo() {
        std::cout << "~demo()\n";
    }
};


class myexception : public std::exception {
    std::string m_msg;

public:
    myexception(const std::string& msg)
        : m_msg(msg)
    {
        std::cout << "myexception()\n";
    }


    virtual ~myexception() {
        std::cout << "~myexception()\n";
    }


    virtual const char *what() const {
        return m_msg.c_str();
    }
};


// when an exception is thrown
//
// destructor of constructed objects are called
//     after constructor of exception class is called
//     before jumping into the catch block
// 
// exception class
//     constructor is called at the throw statement
//     destructor is called at the end of the appropriate catch block
//     
// so here the output:
//     demo()
//     myexception()
//     ~demo()
//     myexception is thrown.
//     ~myexception()
int main() {
    try {
        demo d;
        throw myexception("myexception is thrown.");
    }
    catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
}
