#include "utility.h"

#include <stdexcept>
#include <string>

#include <windows.h>

#include "utility/utility.h"


MEMORY_BASIC_INFORMATION process::memory::query_info(HANDLE process_handle, std::uintptr_t address) {
    arg_check(process_handle, "process handle cannot be null");

    MEMORY_BASIC_INFORMATION mbi{};
    auto r = VirtualQueryEx(
        process_handle,
        reinterpret_cast<LPCVOID>(address),
        &mbi,
        sizeof(mbi)
    );

    if (r != sizeof(mbi))
        throw make_system_except(GetLastError(), "cannot query memory info");

    return mbi;
}
