#include "process.h"

#include <Psapi.h>


std::vector<DWORD> process::get_pids() {
    std::vector<DWORD> pids(1024);

    DWORD bytes_returned = 0;
    auto r = EnumProcesses(
        pids.data(),
        static_cast<DWORD>(pids.size() * sizeof(decltype(pids)::value_type)),
        &bytes_returned
    );
    if (!r)
        throw make_system_except(GetLastError(), "cannot enum processes");

    pids.resize(bytes_returned / sizeof(decltype(pids)::value_type));
    return pids;
}


std::vector<HMODULE> process::get_modules(DWORD pid) {
    auto process_handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!process_handle)
        throw make_system_except(GetLastError(), "cannot open process");

    auto unique_handle = make_unique_handle(process_handle);

    std::vector<HMODULE> modules(1024);

    DWORD bytes_returned = 0;
    auto r = EnumProcessModulesEx(
        *unique_handle,
        modules.data(),
        static_cast<DWORD>(modules.size() * sizeof(decltype(modules)::value_type)),
        &bytes_returned,
        LIST_MODULES_ALL
    );
    if (!r)
        throw make_system_except(GetLastError(), "cannot enum modules");

    modules.resize(bytes_returned / sizeof(decltype(modules)::value_type));
    return modules;
}
