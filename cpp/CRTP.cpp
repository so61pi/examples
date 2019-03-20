#include <iostream>


template<typename Derived>
class Base {
public:
    void Show() {
        static_cast<Derived *>(this)->DoShow();
    }
};


class Derived1 : public Base < Derived1 >
{
public:
    void DoShow() {
        std::cout << "Derived1::DoShow\n";
    }
};


class Derived2 : public Base < Derived2 >
{
public:
    void DoShow() {
        std::cout << "Derived2::DoShow\n";
    }
};


int main() {
    Derived1 d1;
    Derived2 d2;
    Base<Derived1> *b1 = &d1;
    Base<Derived2> *b2 = &d2;

    b1->Show();
    b2->Show();
}
