#include <iostream>


/*
[Namespace alias]
Namespace aliases are only allowed in namespace scope or block scope.
Namespace aliases are NOT allowed in class scope.


Below are the text and grammar extracted from the Standard Specification.

6.3 Compound statement or block [stmt.block]
1. ... the compound statement (also, and equivalently, called "block")...

> block-declaration:
|   ...
|   namespace-alias-definition
|   ...
|
|---> declaration-statement:
|   |   block-declaration
|   |
|   '---> statement:
|       |   ...
|       |   attribute-specifier-seq[opt] compound-statement
|       |   ...
|       |   declaration-statement
|       |   ...
|       |
|       '---> statement-seq:
|           |   statement
|           |   statement-seq statement
|           |
|           '---> compound-statement:
|                   { statement-seq[opt] }
|
'---> declaration:
    |   block-declaration
    |   ...
    |
    '---> declaration-seq:
        |   declaration
        |   declaration-seq declaration
        |
        '---> namespace-body:
                declaration-seq[opt]
*/


namespace A {
    namespace B {
        namespace C {
            void show() {
                std::cout << "A::B::C::show()" << std::endl;
            }
        } // namespace C
    } // namespace B
} // namespace A


class Test {
public:
    // cannot use namespace alias in class scope
    //
    // namespace ABC = ::A::B::C;

    void show() const {
        // namespace alias as function scope
        namespace AB = A::B;

        AB::C::show();

        {
            // or in nested scope
            namespace ABC = AB::C;

            ABC::show();
        }
    }
};


int main() {
    Test test;
    test.show();
}
