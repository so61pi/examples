#ifndef PROCESS_MEMORY_SCANNER_H
#define PROCESS_MEMORY_SCANNER_H


#include <cstdint>
#include <exception>
#include <memory>
#include <stdexcept>

#include <windows.h>

#include "utility.h"


namespace process { namespace memory {


    //
    // memory scanner
    //
    class scanner {
    public:
        explicit scanner(std::shared_ptr<HANDLE> process_handle) :
            m_process_handle(std::move(process_handle))
        {}


        // go to next memory region
        //
        // handler signature is
        //     bool(MEMORY_BASIC_INFORMATION const&)
        template<typename Function>
        bool next_region(Function&& handler) {
            try {
                auto address = reinterpret_cast<std::uintptr_t>(m_mbi.BaseAddress) + m_mbi.RegionSize;
                m_mbi = query_info(*m_process_handle, address);
                auto const& const_mbi = m_mbi;
                return handler(const_mbi);
            }
            catch (std::runtime_error const&) {
                return false;
            }
        }


        //
        // reset the scanner
        //
        void reset() {
            m_mbi = MEMORY_BASIC_INFORMATION{};
        }


    private:
        std::shared_ptr<HANDLE> m_process_handle;
        MEMORY_BASIC_INFORMATION m_mbi{};
    };


}} // namespace process::memory


#endif // PROCESS_MEMORY_SCANNER_H
