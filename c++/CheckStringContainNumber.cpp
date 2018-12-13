#include <cassert>
#include <cstring>
#include <iostream>


int main() {
    //
    assert(std::strspn("0123abcxyz", "0123") == 4);

    //
    auto str = "456"
    if (strspn(str, "0123456789") == strlen(str)) {
        std::cout << '"' << str << '"' << " only contains numbers.\n";
    }
}
