#ifndef PROCESS_MEMORY_UTILITY_H
#define PROCESS_MEMORY_UTILITY_H


#include <cstdint>

#include <windows.h>


namespace process { namespace memory {


    MEMORY_BASIC_INFORMATION query_info(HANDLE process_handle, std::uintptr_t address);


}} // namespace process::memory


#endif // PROCESS_MEMORY_UTILITY_H
