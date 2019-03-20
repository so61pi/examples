#ifndef TEST_H
#define TEST_H


#include <memory>
#include <string>


class Test {
public:
    // There are 2 places where members' destructors can be called:
    //     - Constructor, in case exception is thrown during constructing.
    //     - Destructor.
    //
    // Therefore both constructor and destructor of Test class
    // must be placed where it can call all members' destructors.
    //
    // For example, if m_str's constructor throws exception,
    // then impl's destructor must be called.
    Test();
    ~Test();

    void DoIt();

private:
    // impl's destructor calls Impl's destructor (of course),
    // when it is invoked, it must know how to call Impl's destructor
    // (because maybe Impl's destructor is deleted or private...).
    // Therefore, Impl must be a complete-type at the placed
    // impl's destructor is called.
    class Impl;
    std::unique_ptr<Impl> impl;

    // for explaining purpose only
    std::string m_str;
};


#endif // TEST_H
