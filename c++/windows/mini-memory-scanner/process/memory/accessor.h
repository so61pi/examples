#ifndef PROCESS_MEMORY_ACCESS_H
#define PROCESS_MEMORY_ACCESS_H


#include <cctype>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>
#include <boost/optional.hpp>

#include <windows.h>

#include "parser/pattern_parser.h"
#include "utility/utility.h"
#include "const_range.h"
#include "range.h"


namespace process { namespace memory {


    //
    //
    //
    class accessor {
    public:
        explicit accessor(std::shared_ptr<HANDLE> process_handle) :
            m_process_handle(std::move(process_handle))
        {}


        //
        // write value to remote process
        //
        template<typename T>
        void write(T *address, T const& value) {
            arg_check(address, "address cannot be null");

            process::memory::range<T> mr{ m_process_handle, address, address + 1 };
            auto it = mr.begin();
            *it = value;
        }


        //
        // write values to remote process
        //
        template<typename T>
        void write(T *address, std::vector<T> const& values) {
            arg_check(address, "address cannot be null");

            process::memory::range<T> mr{ m_process_handle, address, address + values.size() };
            std::copy_n(values.begin(), values.size(), mr.begin());
        }


        //
        // write pattern to remote process
        //
        void write(std::uint8_t *address, std::string const& pattern) {
            auto pat = parse_pattern(pattern);
            process::memory::range<std::uint8_t> mr{ m_process_handle, address, address + pat.size() };
            for (decltype(pat.size()) i = 0; i < pat.size(); ++i) {
                if (pat[i])
                    mr[i] = *pat[i];
            }
        }


        //
        // read value from remote process
        //
        template<typename T>
        T read(T const *address) {
            arg_check(address, "address cannot be null");

            process::memory::const_range<T> mr{ m_process_handle, address, address + 1 };
            auto it = mr.begin();
            return *it;
        }


        //
        // read values from remote process
        //
        template<typename T>
        std::vector<T> read(T const *address, std::size_t length) {
            arg_check(address, "address cannot be null");

            std::vector<T> result;

            process::memory::const_range<T> mr{ m_process_handle, address, address + length };
            for (auto& e : mr)
                result.emplace_back(e);

            return result;
        }


    private:
        std::shared_ptr<HANDLE> m_process_handle;
    };


}} // namespace process::memory


#endif // PROCESS_MEMORY_ACCESS_H
