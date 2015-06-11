#include <iostream>


/*
[Using directive]
Using directives are only allowed in namespace scope or block scope.


Below is the text extracted from the Standard Specification.

6.3 Compound statement or block [stmt.block]
1. ... the compound statement (also, and equivalently, called "block")...

7.3.4 Using directive [namespace.udir]
1. A using-directive shall not appear in class scope, but may appear in namespace scope or in block scope.
*/


namespace A {
    // using directive in namespace scope
    using namespace std;

    void show() {
        cout << "A::show()" << endl;
    }
}


class Test {
public:
    // cannot use using directive in class scope
    //
    // using namespace std;

    void show() const {
        A::show();
    }
};


int main() {
    Test test;
    test.show();
}
