#include <iostream>


template<typename Derived>
class Base {
public:
    void Show() {
        static_cast<Derived *>(this)->Implement();
    }
};


class Derived1 : public Base < Derived1 >
{
    friend class Base < Derived1 > ;

private:
    void Implement() {
        std::cout << "Derived1::Implement\n";
    }
};


class Derived2 : public Base < Derived2 >
{
    friend class Base < Derived2 >;

private:
    void Implement() {
        std::cout << "Derived2::Implement\n";
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
