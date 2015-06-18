#ifndef TEST_H
#define TEST_H


#include <memory>
#include <string>


class Test {
public:
    // if shared_ptr is used instead of unique_ptr, we don't need to
    // provide default destructor in implementation file (Test.cpp)
    // because when shared_ptr is constructed, it also knows how to
    // delete the wrapped object (Impl)
    //
    // read Item 22 in Effective Modern C++ for detail explanation
    Test();

    void DoIt();

private:
    class Impl;
    std::shared_ptr<Impl> impl;
};


#endif // TEST_H
