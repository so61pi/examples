#ifndef PROCESS_H
#define PROCESS_H


#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

#include "utility/utility.h"


namespace process {


    //
    // get process identifier
    //
    std::vector<DWORD> get_pids();


    //
    // get all modules of the given process
    //
    std::vector<HMODULE> get_modules(DWORD pid);


    //
    // walk through all running processes
    //
    // f signature
    //     void(PROCESSENTRY32 const&)
    //
    template<typename Function>
    void walk_processes(Function&& f) {
        auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE)
            throw make_system_except(GetLastError(), "cannot take processes snap shot");

        auto uss = make_unique_handle(snapshot);

        PROCESSENTRY32 pe{};
        pe.dwSize = sizeof(pe);
        if (Process32First(*uss, &pe)) {
            do {
                auto const& const_pe = pe;
                f(const_pe);
            } while (Process32Next(*uss, &pe));
        }
    }


    //
    // walk through all threads in a specific process
    //
    // f signature
    //     void(THREADENTRY32 const&)
    //
    template<typename Function>
    void walk_threads(DWORD pid, Function&& f) {
        auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);
        if (snapshot == INVALID_HANDLE_VALUE)
            throw make_system_except(GetLastError(), "cannot take threads snap shot");

        auto uss = make_unique_handle(snapshot);

        THREADENTRY32 te{};
        te.dwSize = sizeof(te);
        if (Thread32First(*uss, &te)) {
            do {
                auto const& const_te = te;
                f(const_te);
            } while (Thread32Next(*uss, &te));
        }
    }


    //
    // walk through all modules in a specific process
    //
    // f signature
    //     void(MODULEENTRY32 const&)
    //
    template<typename Function>
    void walk_modules(DWORD pid, Function&& f) {
        auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
        if (snapshot == INVALID_HANDLE_VALUE)
            throw make_system_except(GetLastError(), "cannot take modules snap shot");

        auto unique_snapshot = make_unique_handle(snapshot);

        MODULEENTRY32 me{};
        me.dwSize = sizeof(me);
        if (Module32First(*unique_snapshot, &me)) {
            do {
                auto const& const_me = me;
                f(const_me);
            } while (Module32Next(*unique_snapshot, &me));
        }
    }


} // namespace process


#endif // PROCESS_H
