#include <list>
#include <memory>
#include <utility>
#include <vector>


template<template<typename, typename /* for allocator */> class Container, typename Type, typename... Args>
auto make_container(Args&&... args) {
    return Container<Type, std::allocator<Type>>(std::forward<Args>(args)...);
}


int main() {
    make_container<std::vector, int>(10, 0xFF);
    make_container<std::list, char>(10, 'A');
}
