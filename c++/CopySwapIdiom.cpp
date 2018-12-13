#include <algorithm>
#include <cstddef>


class Array {
    std::size_t m_size;
    int *m_array;

public:
    explicit Array(std::size_t size = 0)
        : m_size{ size },
          m_array{ m_size ? new int[m_size]{} : nullptr }
    {}

    ~Array() {
        delete[] m_array;
    }

    Array(const Array& other)
        : Array(other.m_size)
    {
        // all-or-nothing
        //
        // if this constructor throws exception (from std::copy),
        // the destructor will be called (delegating constructor),
        // and all the allocated resources are freed
        //
        // else if this doesn't throw, then we're done

        std::copy(other.m_array, other.m_array + other.m_size, m_array);
    }

    Array(Array&& other)
        : Array()
    {
        // just swap 2 objects
        swap(other);
    }

    // pass-by-value assignment operator
    // read more for the reason:
    //     https://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
    Array& operator=(Array rhs) {
        swap(rhs);
        return *this;
    }

    // no-fail swap
    void swap(Array& rhs) {
        // swap the sizes
        std::swap(m_size, rhs.m_size);

        // just swap the pointers, not the actual content
        // then, no-fail
        std::swap(m_array, rhs.m_array);
    }
};


int main() {
    Array a;
    Array b(10);
    a = b;

    Array c = a;
}
