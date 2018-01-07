#ifndef PROCESS_MEMORY_RANGE_H
#define PROCESS_MEMORY_RANGE_H


#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include <windows.h>

#include "utility/utility.h"
#include "const_range.h"
#include "utility.h"


namespace process { namespace memory {


    namespace detail { namespace range {


        //
        //
        //
        template<typename T>
        class const_iterator_t;

        template<typename T>
        class iterator_t;


        //
        //
        //
        template<typename T>
        class const_reference_t;

        template<typename T>
        class reference_t;


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
        };

        template<typename T>
        struct iterator_traits<iterator_t<T>> : iterator_traits<void> {
            using value_type = T;
            using reference_type = reference_t<value_type>;
            using pointer_type = value_type *;
            using iterator_type = iterator_t<T>;
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
        };

        template<typename T>
        struct reference_traits<reference_t<T>> {
            using value_type = T;
            using pointer_type = value_type *;
            using iterator_type = iterator_t<value_type>;
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


        public:
            explicit base_reference_t(HANDLE process_handle, pointer_type address) :
                m_process_handle(process_handle),
                m_address(address)
            {
                arg_check(m_process_handle, "process handle cannot be null");
            }


            // cannot implement this function in const_reference_t class
            operator value_type() {
                std::vector<std::uint8_t> buffer(sizeof(value_type));

                if (!ReadProcessMemory(m_process_handle, reinterpret_cast<LPCVOID>(m_address), reinterpret_cast<LPCVOID>(buffer.data()), buffer.size(), nullptr))
                    throw make_system_except(GetLastError(), "cannot read process memory");

                return *reinterpret_cast<value_type *>(buffer.data());
            }


            // return an iterator instead of real address
            iterator_type operator&() {
                return iterator_type{ m_process_handle, m_address };
            }


        protected:
            HANDLE get_process_handle() {
                return m_process_handle;
            }

            pointer_type get_address() {
                return m_address;
            }


        public:
            HANDLE m_process_handle;
            pointer_type m_address;
        };


        //
        // present a reference to a T const in remote process
        //
        template<typename T>
        class const_reference_t : public base_reference_t<const_reference_t<T>> {
        public:
            using pointer_type = typename reference_traits<const_reference_t<T>>::pointer_type;


        public:
            explicit const_reference_t(HANDLE process_handle, pointer_type address) :
                base_reference_t<const_reference_t<T>>(process_handle, address)
            {}


            // cannot assign value to reference to const
            void operator=(const_reference_t const&) = delete;
        };


        //
        // present a reference to a T in remote process
        //
        template<typename T>
        class reference_t : public base_reference_t<reference_t<T>> {
        public:
            using value_type = typename reference_traits<reference_t<T>>::value_type;
            using pointer_type = typename reference_traits<reference_t<T>>::pointer_type;


        public:
            explicit reference_t(HANDLE process_handle, pointer_type address) :
                base_reference_t<reference_t<T>>(process_handle, address)
            {}


            // assign value
            reference_t& operator=(value_type const& value) {
                if (!WriteProcessMemory(this->get_process_handle(), reinterpret_cast<LPVOID>(this->get_address()), reinterpret_cast<LPCVOID>(&value), sizeof(value), nullptr))
                    throw make_system_except(GetLastError(), "cannot write process memory");

                return *this;
            }


            // assign value from another reference
            reference_t& operator=(reference_t const& other) {
                *this = static_cast<value_type>(other);
                return *this;
            }


            // convert to const_reference_t
            operator const_reference_t<T>() {
                return const_reference_t<T>{ this->get_process_handle(), this->get_address() };
            }
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
            using iterator_type = typename iterator_traits<DerivedIterator>::iterator_type;
            using this_type = base_iterator_t;


        public:
            base_iterator_t() = default;

            explicit base_iterator_t(HANDLE process_handle, pointer_type address) :
                m_process_handle(process_handle),
                m_address(address)
            {
                arg_check(m_process_handle, "process handle cannot be null");
            }


            // *it
            reference_type operator*() {
                return reference_type{ m_process_handle, m_address };
            }

            // ++it
            iterator_type& operator++() {
                ++m_address;
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
                --m_address;
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
                m_address += n;
                return *static_cast<iterator_type *>(this);
            }

            // it -= n
            iterator_type& operator-=(difference_type n) {
                m_address -= n;
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
            reference operator[](difference_type index) {
                return reference{ m_process_handle, m_address + index };
            }

            // it == other
            bool operator==(this_type const& other) const {
                compatible_check(*this, other);
                return m_address == other.m_address;
            }

            // it != other
            // base on it == other
            bool operator!=(this_type const& other) const {
                return !(*this == other);
            }

            // it < other
            bool operator<(this_type const& other) const {
                compatible_check(*this, other);
                return m_address < other.m_address;
            }

            // it > other
            bool operator>(this_type const& other) const {
                compatible_check(*this, other);
                return m_address > other.m_address;
            }

            // it <= other
            // base on it > other
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
                return m_address;
            }


        protected:
            HANDLE get_process_handle() {
                return m_process_handle;
            }


            pointer_type get_address() {
                return m_address;
            }


        private:
            static void compatible_check(this_type const& a, this_type const& b) {
                // check handle
                arg_check(a.m_process_handle == b.m_process_handle, "wrong process handle");
            }


        private:
            HANDLE m_process_handle{};
            pointer_type m_address{};
        };


        //
        // iterator of T const in remote process
        //
        template<typename T>
        class const_iterator_t : public base_iterator_t<const_iterator_t<T>> {
        public:
            using pointer_type = typename iterator_traits<const_iterator_t<T>>::pointer_type;


        public:
            const_iterator_t() :
                base_iterator_t<const_iterator_t<T>>()
            {}

            explicit const_iterator_t(HANDLE process_handle, pointer_type address) :
                base_iterator_t<const_iterator_t<T>>(process_handle, address)
            {}
        };


        //
        // iterator of T in remote process
        //
        template<typename T>
        class iterator_t : public base_iterator_t<iterator_t<T>> {
        public:
            using pointer_type = typename iterator_traits<iterator_t<T>>::pointer_type;


        public:
            iterator_t() :
                base_iterator_t<iterator_t<T>>()
            {}

            explicit iterator_t(HANDLE process_handle, pointer_type address) :
                base_iterator_t<iterator_t<T>>(process_handle, address)
            {}


            // convert to const_iterator_t<T>
            operator const_iterator_t<T>() {
                return const_iterator_t<T>{ this->get_process_handle(), this->get_address() };
            }
        };


    }} // namespace detail::range


    //
    // present a range of memory in remote process
    // 
    // T cannot be a const type
    //
    template<typename T>
    class range {
    public:
        using value_type = T;
        using pointer_type = value_type *;
        using iterator_type = detail::range::iterator_t<value_type>;
        using const_iterator_type = detail::range::const_iterator_t<value_type>;
        using reference_type = typename detail::range::iterator_traits<iterator_type>::reference_type;
        using const_reference_type = typename detail::range::iterator_traits<const_iterator_type>::reference_type;
        using difference_type = typename detail::range::iterator_traits<iterator_type>::difference_type;


    public:
        // memory in range [begin, end) must be accessible
        explicit range(std::shared_ptr<HANDLE> process_handle, pointer_type begin, pointer_type end) :
            m_process_handle(std::move(process_handle)),
            m_begin(begin),
            m_end(end)
        {
            arg_check(m_process_handle, "invalid process handle");
            arg_check(m_begin && m_begin <= m_end, "invalid pointer");
            arg_check((m_end - m_begin) % sizeof(value_type) == 0, "invalid pointer");
        }


        iterator_type begin() {
            return iterator_type{ *m_process_handle, m_begin };
        }


        iterator_type end() {
            return iterator_type{ *m_process_handle, m_end };
        }


        const_iterator_type begin() const {
            return const_iterator_type{ *m_process_handle, m_begin };
        }


        const_iterator_type end() const {
            return const_iterator_type{ *m_process_handle, m_end };
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


        // convert to const_range<T>
        operator const_range<T>() {
            return const_range<T>{ m_process_handle, m_begin, m_end };
        }


    private:
        std::shared_ptr<HANDLE> m_process_handle;
        pointer_type m_begin;
        pointer_type m_end;
    };


}} // namespace process::memory


#endif // PROCESS_MEMORY_RANGE_H
