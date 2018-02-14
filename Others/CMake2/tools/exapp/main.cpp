#include <iostream>

#include "example/libexample.h"
#include "config.h"


int main() {
    std::cout << CONFIG_STRING << std::endl;
    show();
}
