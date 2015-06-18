#include "Test.h"

#include <iostream>
#include <string>


class Test::Impl {
public:
    Impl(const std::string& data)
        : m_data(data)
    {}

    void Show() {
        std::cout << m_data;
    }

    // data is public so class Test can use them directly
    const std::string m_data;
};


Test::Test()
    : impl(std::make_unique<Impl>("hello\n"))
{}


Test::~Test() = default;


void Test::DoIt() {
    impl->Show();
}
