#include <iostream>
#include <memory>
#include <new>


class example {
public:
    example() {
        std::cout << "example()\n";
    }


    ~example() {
        std::cout << "~example()\n";
    }


    // class-specific operator new
    // new-expression calls this function to allocate memory
    // before calling object constructor
    static void *operator new(std::size_t size) {
        std::cout << "example::operator new\n";
        return std::malloc(size);
    }


    // class-specific operator delete
    // delete-expression calls this function to deallocate memory
    // after calling object destructor
    static void operator delete(void *p) {
        std::cout << "example::operator delete\n";
        std::free(p);
    }
};


// the new-expression calls operator new to allocate memory
// then calls the object constructor on allocated memory
//
// the delete-expression calls the object destructor
// then calls operator delete to deallocate memory
//
// the output
//     example::operator new
//     example()
//     ~example()
//     example::operator delete
int main() {
    // use class-specific operator new
    example *e = new example;

    // use class-specific operator delete
    delete e;
}
