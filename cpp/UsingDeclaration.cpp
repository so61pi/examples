#include <iostream>
#include <memory>
#include <string>


/*
[The using declaration]
Create (template) type alias
    Can be used in namespace scope, class scope, and block scope.
    e.g.
        using string = std::string;

        template<typename T>
        using pointer_t = std::unique_ptr<T>;

Introduce Base class functions into Derived class
    Can be used in class scope.
    e.g.
        using Base::show;

Introduce a name from another namespace into current scope
    Can be used in namespace scope, and block scope.
    e.g.
        using std::cout;


Below is the text extracted from the Standard Specification.

7.3.3 The using declaration [namespace.udecl]
2. Every using-declaration is a declaration and a member-declaration and so can be used in a class definition.
5. A using-declaration shall not name a template-id.
6. A using-declaration shall not name a namespace.
7. A using-declaration shall not name a scoped enumerator.
8. A using-declaration for a class member shall be a member-declaration.
*/


class Base {
public:
    void show(int) const {
        // introduce std::cout into current scope
        using std::cout;

        cout << "Base::show(int)" << std::endl;
    }

    template<typename T>
    void show(const T&) const {
        std::cout << "Base::show(const T&)" << std::endl;
    }
};


class Derived : public Base
{
    // create a type alias for std::string
    using string = std::string;

public:
    // introduce all show functions from the Base class into this class
    // or the function show(const string&) will hide show(int) and show(const T&)
    // in the Base class (Name Lookup rules)
    using Base::show;

    void show(const string&) const {
        std::cout << "Derived::show(const string&)" << std::endl;
    }
};


// create a template type alias for std::unique_ptr
template<typename T>
using pointer_t = std::unique_ptr<T>;


int main() {
    Derived d;

    // call Base::show(int)
    d.show(65);

    // call Base::show(const T&)
    d.show("hello");

    // call Derived::show(const string&)
    d.show(std::string{ "hello" });

    // use template type alias
    pointer_t<int> pointer = std::make_unique<int>();
}
