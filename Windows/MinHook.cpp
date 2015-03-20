#include <iostream>
#include <Windows.h>

#include "MinHook.h"


using ADD_PROTO = int (*)(int, int);
ADD_PROTO g_Add = nullptr;


// hooked function
int Add(int a, int b) {
    return a + b;
}


// in case you hook another application, you can use OutputDebugString
// to send a string to system debugger (DbgView can be used to view those strings)
int AddDetour(int a, int b) {
    if (!g_Add)
        return 0;

    // change the arguemnts
    int result = g_Add(a + 1, b + 1);

    // change the result
    ++result;

    return result;
}


int main() {
    // call this function only once before using MinHook
    MH_Initialize();

    // create & enable hook
    MH_CreateHook((LPVOID)&Add, (LPVOID)&AddDetour, (LPVOID *)&g_Add);
    MH_EnableHook((LPVOID)&Add);

    // now the result is (1+1)+(2+1)+1 = 6 instead of 3
    std::cout << Add(1, 2) << std::endl;

    // call this function when application exits
    MH_Uninitialize();
}
