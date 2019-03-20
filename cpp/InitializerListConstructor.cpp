#include <iostream>
#include <initializer_list>


/*
13.3.1.7 Initialization by list-initialization [over.match.list]
 
1. When objects of non-aggregate class type T are list-initialized such
that 8.5.4 specifies that overload resolution is performed according to
the rules in this section, overload resolution selects the constructor
in two phases:

    (1.1) — Initially, the candidate functions are the initializer-list
    constructors (8.5.4) of the class T and the argument list consists
    of the initializer list as a single argument.

    (1.2) — If no viable initializer-list constructor is found, overload
    resolution is performed again, where the candidate functions are all
    the constructors of the class T and the argument list consists of the
    elements of the initializer list.

If the initializer list has no elements and T has a default constructor,
the first phase is omitted. In copy-list initialization, if an explicit
constructor is chosen, the initialization is ill-formed.

[ Note: This differs from other situations (13.3.1.3, 13.3.1.4), where
only converting constructors are considered for copy-initialization.
This restriction only applies if this initialization is part of the
final result of overload resolution. —end note ]
*/

/*
In short, initializer-list constructor is preferred to all non-initializer-list constructors.
*/


class ClassA {
public:
    ClassA() {
        std::cout << "ClassA::ClassA()" << std::endl;
    }

    ClassA(const std::initializer_list<int>&) {
        std::cout << "ClassA::ClassA(const std::initializer_list<int>&)" << std::endl;
    }

    ClassA(int) {
        std::cout << "ClassA::ClassA(int)" << std::endl;
    }
};


class ClassB
{
public:
    template<typename T>
    ClassB(const T&) {
        std::cout << "ClassB::ClassB(const T&)" << std::endl;
    }

    ClassB(const std::initializer_list<ClassB>&) {
        std::cout << "ClassB::ClassB(const std::initializer_list<int>&)" << std::endl;
    }

    ClassB(const ClassB&) {
        std::cout << "ClassB::ClassB(const ClassB&)" << std::endl;
    }
};


int main() {
    // calls ClassA::ClassA(const std::initializer_list<int>&)
    // instead of ClassA::ClassA(int)
    ClassA a1{ 1 };

    // calls ClassA::ClassA(int)
    ClassA a2(1);


    // first, constructs a std::initializer_list<ClassB> by
    //     1. calls ClassB::ClassB(const T&) to convert const char * to ClassB
    //     2. constructs a std::initializer_list<ClassB> from the constructed ClassB from #1
    // then, calls ClassB::ClassB(const std::initializer_list<int>&)
    ClassB b1{ "ABC" };

    // same as above
    ClassB b2{ b1 };

    // calls ClassB::ClassB(const ClassB&)
    ClassB b3(b2);
}
