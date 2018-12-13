#include <cstddef>
#include <iostream>
#include <memory>


// heap only object with std::shared_ptr
// use private constructor
class heap_only_t_1 {
public:
    static auto make(int data) {
        // cannot use std::make_shared
        return std::shared_ptr<heap_only_t_1>{ new heap_only_t_1{ data } };
    }

    heap_only_t_1(heap_only_t_1 const&) = delete;
    heap_only_t_1(heap_only_t_1&&) = delete;

private:
    heap_only_t_1(int)
    {}
};


// heap only object with std::shared_ptr
// use hide tag, this makes constructor uncallable
class heap_only_t_2 {
private:
    struct hide_tag {};

public:
    static auto make(int data) {
        return std::make_shared<heap_only_t_2>(data, hide_tag{});
    }

    heap_only_t_2(heap_only_t_2 const&) = delete;
    heap_only_t_2(heap_only_t_2&&) = delete;

    // no one outside this class can use this constructor
    heap_only_t_2(int, hide_tag)
    {}
};


// stack only object
//
// it can prevent "normal" new,
// but people can still use global allocation function:
//     ::new stack_only_t
// hope no one will do that
class stack_only_t {
public:
    void *operator new(std::size_t) = delete;
    void *operator new[](std::size_t) = delete;
};


int main() {
    auto hot1 = heap_only_t_1::make(1);
    auto hot2 = heap_only_t_2::make(2);
    stack_only_t sot;
}
