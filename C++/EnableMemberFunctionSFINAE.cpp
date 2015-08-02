#include <iostream>
#include <type_traits>


template<typename T>
class Test {
private:
    struct delay_tag {};

public:
    /*
    Enable function bases on class' template parameters.

    Because Delay can be provided when calls show() function, std::enable_if is not "executed"
    right away.

    std::is_same check if Delay is delay_tag or not and delay_tag is private so user cannot
    touch Delay parameter.
    */
    template<
        typename Delay = delay_tag,
        std::enable_if_t<
            std::is_pointer<T>::value && std::is_same<Delay, delay_tag>::value
        > * = nullptr
    >
    void show() {
        std::cout << "pointer\n";
    }

    template<
        typename Delay = delay_tag,
        std::enable_if_t<
            std::is_const<T>::value && std::is_same<Delay, delay_tag>::value
        > * = nullptr
    >
    void show() {
        std::cout << "const\n";
    }

    /*
    Function templates can have identical function return type and function parameter lists.

    14.5.6.1 Function template overloading [temp.over.link]

    3. The signature of a function template is defined in 1.3. The names of the template 
    parameters are significant only for establishing the relationship between the template 
    parameters and the rest of the signature. [ Note: Two distinct function templates may 
    have identical function return types and function parameter lists, even if overload 
    resolution alone cannot distinguish them.

        template<class T> void f();
        template<int I> void f();   // OK: overloads the first template
                                    // distinguishable with an explicit template argument list

    —end note ]
    */
};


int main() {
    Test<int const> a{};
    Test<int *> b{};
    a.show();
    b.show();
}
