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
};


// customized operator new
// new-expression calls this function to allocate memory
// before calling object constructor
void *operator new(std::size_t size){
    std::cout << "operator new\n";
    return std::malloc(size);
}


// customized operator delete
// delete-expression calls this function to deallocate memory
// after calling object destructor
void operator delete(void *p) {
    std::cout << "operator delete\n";
    std::free(p);
}


// the new-expression calls operator new to allocate memory
// then calls the object constructor on allocated memory
//
// the delete-expression calls the object destructor
// then calls operator delete to deallocate memory
//
// the output
//     operator new
//     example()
//     ~example()
//     operator delete
int main() {
    example *e = new example;
    delete e;
}
