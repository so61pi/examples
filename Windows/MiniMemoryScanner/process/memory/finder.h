#ifndef PROCESS_MEMORY_FINDER_H
#define PROCESS_MEMORY_FINDER_H


#include <cctype>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>

#include <windows.h>

#include "parser/pattern_parser.h"
#include "utility/utility.h"
#include "const_range.h"
#include "scanner.h"


namespace process { namespace memory {


    class finder {
    public:
        explicit finder(std::shared_ptr<HANDLE> process_handle) :
            m_process_handle(std::move(process_handle)),
            m_scanner(m_process_handle)
        {}


        //
        // find a pattern
        //
        // e.g.
        //     std::string pattern = "11 ?? 33";
        //     find(pattern, mem_predicate);
        //
        template<typename Function>
        std::vector<std::uint8_t const *> find(std::string const& pattern, Function&& mem_predicate) {
            auto pat = parse_pattern(pattern);
            auto data_bounds = std::make_pair(pat.begin(), pat.end());
            auto comparator = [](typename process::memory::const_range<std::uint8_t>::reference_type a, boost::optional<std::uint8_t> const& b) -> bool {
                if (b) {
                    return a == *b;
                }
                return true;
            };

            std::vector<std::uint8_t const *> result;
            find_impl(result, data_bounds, mem_predicate, comparator);
            return result;
        }


        //
        // find std::vector<T>
        //
        // e.g.
        //     std::vector<std::uint8_t> data{ 0x11, 0x22, 0x33, 0x44 };
        //     find(data, mem_predicate);
        //
        template<typename T, typename Function>
        std::vector<T const *> find(std::vector<T> const& data, Function&& mem_predicate) {
            return find(data.data(), data.size(), std::forward<Function>(mem_predicate));
        }


        //
        // find a single value
        //
        // e.g.
        //     DWORD data = 0x11223344;
        //     find(data, mem_predicate);
        //
        template<typename T, typename Function>
        std::vector<T const *> find(T const& data, Function&& mem_predicate) {
            return find(&data, 1, std::forward<Function>(mem_predicate));
        }


        //
        // find an array
        //
        // e.g.
        //     DWORD data[] = { 0x11, 0x22, 0x33, 0x44 };
        //     find(data, mem_predicate);
        //
        template<typename T, std::size_t N, typename Function>
        std::vector<T const *> find(T const(&ref)[N], Function&& mem_predicate) {
            return find(ref, N, std::forward<Function>(mem_predicate));
        }


        //
        // find an array
        //
        // mem_predicate signature is
        //     bool(MEMORY_BASIC_INFORMATION const&)
        //
        // e.g.
        //     DWORD data[] = { 0x11, 0x22, 0x33, 0x44 };
        //     DWORD *temp = data;
        //     find(temp, 4, mem_predicate);
        //
        template<typename T, typename Function>
        std::vector<T const *> find(
            T const *data,
            std::size_t size, // in elements
            Function&& mem_predicate
        )
        {
            arg_check(data, "data cannot be null");

            auto const begin_data = reinterpret_cast<std::uintptr_t>(data);
            auto const end_data = begin_data + size * sizeof(T);

            auto data_bounds = std::make_pair(reinterpret_cast<std::uint8_t *>(begin_data), reinterpret_cast<std::uint8_t *>(end_data));
            auto comparator = std::equal_to<std::uint8_t>{};

            std::vector<T const *> result;
            find_impl(result, data_bounds, mem_predicate, comparator);
            return result;
        }


    private:
        template<typename T, typename DataBound, typename MemPredicate, typename Comparator>
        void find_impl(std::vector<T const *>& result, DataBound const& data_bounds, MemPredicate&& mem_predicate, Comparator&& comparator) {
            auto handler = [this, &result, &data_bounds, &mem_predicate, &comparator](MEMORY_BASIC_INFORMATION const& mbi) -> bool {
                if (mem_predicate(mbi)) {
                    // compute begin & end of remote address
                    auto const begin_mem = reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
                    auto const end_mem = begin_mem + mbi.RegionSize;

                    process::memory::const_range<std::uint8_t> mem_range{ this->m_process_handle, reinterpret_cast<std::uint8_t *>(begin_mem), reinterpret_cast<std::uint8_t *>(end_mem) };
                    auto it = mem_range.cbegin();
                    auto em = mem_range.cend();

                    while ((it = std::search(it, em, data_bounds.first, data_bounds.second, comparator)) != em) {
                        result.push_back(reinterpret_cast<T const *>(static_cast<std::uint8_t const *>(it)));
                        std::advance(it, std::distance(data_bounds.first, data_bounds.second));
                    }
                }

                return true;
            };

            while (m_scanner.next_region(handler)) {
                ;
            }
        }


    private:
        std::shared_ptr<HANDLE> m_process_handle;
        scanner m_scanner;
    };


}} // namespace process::memory


#endif // PROCESS_MEMORY_FINDER_H
