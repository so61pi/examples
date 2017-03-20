#include <iostream>

#include "ArrayBegin.h"
#include "ArrayEnd.h"
#include "Entry.h"


/*
 * g++ -c ArrayEntry1.cpp
 * g++ -c ArrayEntry2.cpp
 * g++ -c main.cpp
 *
 * g++ -Xlinker -T"script.lds" main.o ArrayEntry1.o ArrayEntry2.o
 */


void PrintArray(Entry const* first, Entry const* last) {
    for (; first != last; ++first) {
        std::cout << "Entry\n"
                  << "    name   : " << first->name   << "\n"
                  << "    length : " << first->length << "\n";
    }
}


int main() {
    std::cout << &ArrayBegin << " - " << &ArrayEnd << std::endl;
    PrintArray(reinterpret_cast<Entry const*>(ArrayBegin),
               reinterpret_cast<Entry const*>(ArrayEnd));
}