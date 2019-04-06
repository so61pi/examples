#include <iostream>
#include <string>

#include "obfstr.h"


inline
bool valid(std::string const& key) {
    return key == OBFSTR("1234567890");
}


int main() {
    std::cout << "Type 'quit' to stop.\n\n";

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line == "quit") break;

        if (valid(line)) {
            std::cout << OBFSTR("\tCongratulations, it's correct!\n\n");
            break;
        } else {
            std::cout << OBFSTR("\tInvalid key, please try again!\n\n");
        }
    }
}
