/*

See Pimpl2 for more explanations.

*/

#ifndef TEST_H
#define TEST_H


#include <memory>


// The clients don't know the internal implementation of this class
// so if its behavior is changed (add more private data or functions)
// the client's code doesn't need to recompiled.
class Test {
public:
    Test();
    ~Test();

    void DoIt();

private:
    // Impl must be implemented in Test.cpp file
    class Impl;
    std::unique_ptr<Impl> impl;
};


#endif // TEST_H
