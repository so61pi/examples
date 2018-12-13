#include <sstream>
#include <intrin.h>
#include <Windows.h>

#include "MinHook.h"


using MESSAGEBOXW_PROTO = int (WINAPI *)(HWND, LPCWSTR, LPCWSTR, UINT);
MESSAGEBOXW_PROTO g_MessageBoxW = nullptr;


int WINAPI MessageBoxWDetour(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType) {
    if (!g_MessageBoxW)
        return 0;

    // _ReturnAddress used to identify which function is calling MessageBoxW
    std::wostringstream outputString;
    outputString << L"Return address: " << _ReturnAddress();
    OutputDebugStringW(outputString.str().c_str());

    return g_MessageBoxW(hWnd, lpText, lpCaption, uType);
}


void HookMessageBoxW() {
    auto msgBox = GetProcAddress(GetModuleHandle(L"user32.dll"), "MessageBoxW");
    MH_CreateHook((LPVOID)msgBox, (LPVOID)&MessageBoxWDetour, (LPVOID *)&g_MessageBoxW);
    MH_EnableHook((LPVOID)msgBox);
}


int main() {
    MH_Initialize();

    HookMessageBoxW();

    MessageBoxW(nullptr, L"Text", L"Caption", MB_ICONINFORMATION | MB_OK);

    MH_Uninitialize();
}
