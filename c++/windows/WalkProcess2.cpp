#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


void PrintProcessList();
void PrintModuleList(DWORD processId);
void PrintThreadList(DWORD processId);


int main() {
    // print out all processes
    PrintProcessList();

    // print out all modules in this process
    PrintModuleList(0);

    // print out all threads in all processes
    PrintThreadList(0);
}


// print all processes
void PrintProcessList() {
    auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe{};
        pe.dwSize = sizeof(pe);
        if (Process32First(hSnapshot, &pe) != FALSE) {
            do {
                std::wcout << pe.szExeFile << "\n";
            } while (Process32Next(hSnapshot, &pe) != FALSE);
        }
        CloseHandle(hSnapshot);
    }
}


// print all modules in the given process
void PrintModuleList(DWORD processId) {
    auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 me{};
        me.dwSize = sizeof(me);
        if (Module32First(hSnapshot, &me) != FALSE) {
            do {
                std::wcout << me.szExePath << "\n";
            } while (Module32Next(hSnapshot, &me) != FALSE);
        }
        CloseHandle(hSnapshot);
    }
}


// print all threads in the given process
void PrintThreadList(DWORD processId) {
    auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, processId);
    if (hSnapshot != INVALID_HANDLE_VALUE) {
        THREADENTRY32 te{};
        te.dwSize = sizeof(te);
        if (Thread32First(hSnapshot, &te) != FALSE) {
            do {
                std::wcout << te.th32ThreadID << "\n";
            } while (Thread32Next(hSnapshot, &te) != FALSE);
        }
        CloseHandle(hSnapshot);
    }
}
