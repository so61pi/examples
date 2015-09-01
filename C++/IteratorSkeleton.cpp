#include <iterator>


class iterator_skeleton : public std::iterator<...>
{
public:
    using my_type = iterator_skeleton;

    // member and pointer operators
    // (underlying object-related functions)
    reference operator*() const noexcept {
        return (*this)[0];
    }
    pointer operator->() const noexcept;
    reference operator[](difference_type n) const noexcept;
    
    // iterator arithmetic operators
    auto operator++() noexcept -> my_type&;
    auto operator--() noexcept -> my_type&;
    auto operator++(int) noexcept -> my_type {
        auto temp = *this;
        ++*this;
        return temp;
    }

    auto operator--(int) noexcept -> my_type {
        auto temp = *this;
        --*this;
        return temp;
    }

    auto operator+(difference_type n) const noexcept -> my_type;
    auto operator-(difference_type n) const noexcept -> my_type {
        return *this + (-n);
    }
    auto operator+=(difference_type n) noexcept -> my_type&;
    auto operator-=(difference_type n) noexcept -> my_type& {
        return *this += (-n);
    }

    friend auto operator+(difference_type n, iterator_skeleton const& it) noexcept -> iterator_skeleton {
        return it + n;
    }

    friend auto operator-(iterator_skeleton const& other) const noexcept -> difference_type;
    
    // iterator comparison operators
    bool operator==(my_type const& other) const noexcept;
    bool operator!=(my_type const& other) const noexcept {
        return !(*this == other);
    }

    bool operator<(my_type const& other) const noexcept;
    bool operator<=(my_type const& other) const noexcept {
        return (*this < other) || (*this == other);
    }
    bool operator>(my_type const& other) const noexcept {
        return !(*this <= other);
    }
    bool operator>=(my_type const& other) const noexcept {
        return !(*this < other);
    }
};
