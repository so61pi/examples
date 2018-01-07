#ifndef PROCESS_MEMORY_CONST_RANGE_H
#define PROCESS_MEMORY_CONST_RANGE_H


#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <windows.h>

#include "utility/utility.h"


namespace process { namespace memory {


    namespace detail { namespace const_range {


        //
        //
        //
        template<typename T>
        class const_iterator_t;

        template<typename T>
        using iterator_t = const_iterator_t<T>;


        //
        //
        //
        template<typename T>
        class const_reference_t;

        template<typename T>
        using reference_t = const_reference_t<T>;


        //
        // trait class for iterator type, iterator_t and const_iterator_t
        //
        template<typename Iterator>
        struct iterator_traits {
            using difference_type = std::intptr_t;
            using iterator_category = std::random_access_iterator_tag;
        };

        template<typename T>
        struct iterator_traits<const_iterator_t<T>> : iterator_traits<void> {
            using value_type = T;
            using reference_type = const_reference_t<value_type>;
            using pointer_type = value_type const *;
            using iterator_type = const_iterator_t<T>;
            using container_iterator_type = typename std::vector<value_type>::const_iterator;
        };


        //
        // trait class for reference type, reference_t and const_reference_t
        //
        template<typename Reference>
        struct reference_traits;

        template<typename T>
        struct reference_traits<const_reference_t<T>> {
            using value_type = T;
            using pointer_type = value_type const *;
            using iterator_type = const_iterator_t<value_type>;
            using container_iterator_type = typename std::vector<value_type>::const_iterator;
        };


        //
        // base class for reference_t and const_reference_t
        //
        template<typename DerivedReference>
        class base_reference_t {
        public:
            using value_type = typename reference_traits<DerivedReference>::value_type;
            using pointer_type = typename reference_traits<DerivedReference>::pointer_type;
            using iterator_type = typename reference_traits<DerivedReference>::iterator_type;
            using container_iterator_type = typename reference_traits<DerivedReference>::container_iterator_type;


        public:
            explicit base_reference_t(
                HANDLE process_handle,
                pointer_type base,
                container_iterator_type begin,
                container_iterator_type it
            ) :
                m_process_handle(process_handle),
                m_base(base),
                m_begin(std::move(begin)),
                m_it(std::move(it))
            {
                arg_check(m_process_handle, "process handle cannot be null");
            }


            // return value
            operator value_type() {
                return *m_it;
            }


            // address of
            // return an iterator instead of raw address
            // so we can go to next object by just it++
            iterator_type operator&() {
                return iterator_type{ m_process_handle, m_base, m_begin, m_it };
            }


        private:
            HANDLE m_process_handle;
            pointer_type m_base;
            container_iterator_type m_begin;
            container_iterator_type m_it;
        };


        //
        // present a reference to a T const in remote process
        //
        template<typename T>
        class const_reference_t : public base_reference_t<const_reference_t<T>> {
        public:
            using pointer_type = typename reference_traits<const_reference_t<T>>::pointer_type;
            using container_iterator_type = typename reference_traits<const_reference_t<T>>::container_iterator_type;


        public:
            explicit const_reference_t(
                HANDLE process_handle,
                pointer_type base,
                container_iterator_type begin,
                container_iterator_type it
            ) :
                base_reference_t<const_reference_t<T>>(process_handle, base, std::move(begin), std::move(it))
            {}


            // cannot assign value to reference to const
            void operator=(const_reference_t const&) = delete;
        };


        //
        // base class for iterator_t and const_iterator_t
        //
        template<typename DerivedIterator>
        class base_iterator_t {
        public:
            using value_type = typename iterator_traits<DerivedIterator>::value_type;
            using reference_type = typename iterator_traits<DerivedIterator>::reference_type;
            using reference = reference_type;
            using pointer_type = typename iterator_traits<DerivedIterator>::pointer_type;
            using pointer = pointer_type;
            using difference_type = typename iterator_traits<DerivedIterator>::difference_type;
            using iterator_category = typename iterator_traits<DerivedIterator>::iterator_category;
            using container_iterator_type = typename iterator_traits<DerivedIterator>::container_iterator_type;
            using iterator_type = typename iterator_traits<DerivedIterator>::iterator_type;
            using this_type = base_iterator_t;


        public:
            base_iterator_t() = default;

            explicit base_iterator_t(
                HANDLE process_handle,
                pointer_type base,
                container_iterator_type begin,
                container_iterator_type it
            ) :
                m_process_handle(process_handle),
                m_base(base),
                m_begin(std::move(begin)),
                m_it(std::move(it))
            {
                arg_check(m_process_handle, "process handle cannot be null");
            }


            // *it
            reference_type operator*() const {
                return reference_type{ m_process_handle, m_base, m_begin, m_it };
            }

            // ++it
            iterator_type& operator++() {
                ++m_it;
                return *static_cast<iterator_type *>(this);
            }

            // it++
            // base on ++it
            iterator_type operator++(int) {
                auto temp = *static_cast<iterator_type *>(this);
                ++(*this);
                return temp;
            }

            // --it
            iterator_type& operator--() {
                --m_it;
                return *static_cast<iterator_type *>(this);
            }

            // it--
            // base on --it
            iterator_type operator--(int) {
                auto temp = *static_cast<iterator_type *>(this);
                --(*this);
                return temp;
            }

            // it += n
            iterator_type& operator+=(difference_type n) {
                m_it += n;
                return *static_cast<iterator_type *>(this);
            }

            // it -= n
            iterator_type& operator-=(difference_type n) {
                m_it -= n;
                return *static_cast<iterator_type *>(this);
            }

            // it + n
            // base on it += n
            iterator_type operator+(difference_type n) const {
                auto temp = *static_cast<iterator_type const *>(this);
                temp += n;
                return temp;
            }

            // n + it
            // base on it + n
            friend iterator_type operator+(difference_type n, this_type const& it) {
                return it + n;
            }

            // it - n
            // base on it -= n
            iterator_type operator-(difference_type n) const {
                auto temp = *static_cast<iterator_type const *>(this);
                temp -= n;
                return temp;
            }

            // it[index]
            reference_type operator[](difference_type index) {
                return reference_type{ m_process_handle, m_base, m_begin, m_it + index };
            }

            // it == other
            bool operator==(this_type const& other) const {
                compatible_check(*this, other);
                return m_it == other.m_it;
            }

            // it != other
            // base one it == other
            bool operator!=(this_type const& other) const {
                return !(*this == other);
            }

            // it < other
            bool operator<(this_type const& other) const {
                compatible_check(*this, other);
                return m_it < other.m_it;
            }

            // it > other
            bool operator>(this_type const& other) const {
                compatible_check(*this, other);
                return m_it > other.m_it;
            }

            // it <= other
            // base one it > other
            bool operator<=(this_type const& other) const {
                return !(*this > other);
            }

            // it >= other
            // base on it < other
            bool operator>=(this_type const& other) const {
                return !(*this < other);
            }


        public:
            // convert to raw pointer
            operator pointer_type() const {
                auto temp = reinterpret_cast<difference_type>(m_base)+std::distance(m_begin, m_it);
                return reinterpret_cast<pointer_type>(temp);
            }


        private:
            static void compatible_check(this_type const& a, this_type const& b) {
                // check handle
                arg_check(a.m_process_handle == b.m_process_handle, "wrong process handle");

                // check base
                arg_check(a.m_base == b.m_base, "wrong base address");
            }


        private:
            HANDLE m_process_handle{};
            pointer_type m_base{};
            container_iterator_type m_begin{};
            container_iterator_type m_it{};
        };


        //
        // iterator of T const in remote process
        //
        template<typename T>
        class const_iterator_t : public base_iterator_t<const_iterator_t<T>> {
        public:
            using pointer_type = typename const_iterator_t<T>::pointer_type;
            using container_iterator_type = typename const_iterator_t<T>::container_iterator_type;


        public:
            const_iterator_t() :
                base_iterator_t<const_iterator_t<T>>()
            {}

            explicit const_iterator_t(
                HANDLE process_handle,
                pointer_type base,
                container_iterator_type begin,
                container_iterator_type it
            ) :
                base_iterator_t<const_iterator_t<T>>(process_handle, base, begin, it)
            {}
        };


    }} // namespace detail::const_range


    //
    // present a range of immutable memory in remote process
    //
    // T cannot be a const type
    //
    template<typename T>
    class const_range {
    public:
        using value_type = T;
        using pointer_type = value_type const *;
        using iterator_type = detail::const_range::iterator_t<value_type>;
        using const_iterator_type = detail::const_range::const_iterator_t<value_type>;
        using reference_type = typename detail::const_range::iterator_traits<iterator_type>::reference_type;
        using const_reference_type = typename detail::const_range::iterator_traits<const_iterator_type>::reference_type;
        using difference_type = typename detail::const_range::iterator_traits<iterator_type>::difference_type;


    public:
        explicit const_range(std::shared_ptr<HANDLE> process_handle, pointer_type begin, pointer_type end) :
            m_process_handle(std::move(process_handle)),
            m_begin(begin),
            m_end(end),
            m_buffer((m_end - m_begin) / sizeof(value_type))
        {
            arg_check(m_process_handle, "process handle cannot be null");
            arg_check(m_begin && m_begin <= m_end, "invalid pointer");
            arg_check((m_end - m_begin) % sizeof(value_type) == 0, "invalid pointer");

            // load memory data from other process
            if (!ReadProcessMemory(*this->m_process_handle, reinterpret_cast<LPCVOID>(m_begin), reinterpret_cast<LPVOID>(m_buffer.data()), m_buffer.size() * sizeof(value_type), nullptr))
                throw make_system_except(GetLastError(), "cannot read memory");
        }


        iterator_type begin() {
            return iterator_type{ *m_process_handle, m_begin, m_buffer.cbegin(), m_buffer.cbegin() };
        }


        iterator_type end() {
            return iterator_type{ *m_process_handle, m_begin, m_buffer.cbegin(), m_buffer.cend() };
        }


        //
        const_iterator_type begin() const {
            return const_iterator_type{ *m_process_handle, m_begin, m_buffer.cbegin(), m_buffer.cbegin() };
        }


        const_iterator_type end() const {
            return const_iterator_type{ *m_process_handle, m_begin, m_buffer.cbegin(), m_buffer.cend() };
        }


        const_iterator_type cbegin() const {
            return begin();
        }


        const_iterator_type cend() const {
            return end();
        }


        reference_type operator[](difference_type n) {
            return *(begin() + n);
        }


        const_reference_type operator[](difference_type n) const {
            return *(begin() + n);
        }


    private:
        std::shared_ptr<HANDLE> m_process_handle;
        pointer_type m_begin;
        pointer_type m_end;
        std::vector<value_type> m_buffer;
    };


}} // namespace process::memory


#endif // PROCESS_MEMORY_CONST_RANGE_H
