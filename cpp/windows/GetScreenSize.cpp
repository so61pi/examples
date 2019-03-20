#include <iostream>
#include <Windows.h>


int main() {
    auto xScreen = GetSystemMetrics(SM_CXSCREEN);
    auto yScreen = GetSystemMetrics(SM_CYSCREEN);
    if (xScreen == 0 || yScreen == 0) {
        MessageBox(nullptr, L"Cannot get screen's size.", L"Message", MB_ICONINFORMATION);
        return -1;
    }

    std::cout << "Screen's width is " << xScreen << " pixels\n"
        << "Screen's height is " << yScreen << " pixels\n";
}
