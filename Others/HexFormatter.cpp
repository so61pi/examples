#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>


class formatter {
public:
    formatter(std::ostream& os)
        : m_os{os} {}


    // format data as hex
    auto hex(std::size_t prepend, std::size_t append, char const* data,
             std::size_t size) const -> formatter const& {
        int i = 0;
        for (; prepend > 0; --prepend, ++i) {
            if (i % 2 == 0 && i != 0) m_os << ' ';
            m_os << "..";
        }

        for (auto j = 0U; j < size; ++j, ++i) {
            if (i % 2 == 0 && i != 0) m_os << ' ';
            m_os << std::setw(2) << std::hex
                 << static_cast<unsigned int>(data[j] & 0xFF);
        }

        for (; append > 0; --append, ++i) {
            if (i % 2 == 0 && i != 0) m_os << ' ';
            m_os << "..";
        }

        return *this;
    }


    // format data
    auto data(std::size_t prepend, std::size_t append, char const* data,
              std::size_t size) const -> formatter const& {
        for (; prepend > 0; --prepend) {
            m_os << ".";
        }

        for (auto i = 0U; i < size; ++i) {
            if (32 <= data[i] && data[i] < 128)
                m_os << data[i];
            else
                m_os << ".";
        }

        for (; append > 0; --append) {
            m_os << ".";
        }

        return *this;
    }


    // format address
    auto address(std::size_t addr) const -> formatter const& {
        m_os << std::setw(8) << std::hex << addr;
        return *this;
    }


    // address - hex fence
    auto ahfence() const -> formatter const& {
        m_os << "  |  ";
        return *this;
    }


    // hex - data fence
    auto hdfence() const -> formatter const& {
        return ahfence();
    }


    //
    auto line(std::size_t addr, std::size_t prepend, std::size_t append,
              char const* data, std::size_t size) const -> formatter const& {
        return  address(addr)
               .ahfence()
               .hex(prepend, append, data, size)
               .hdfence()
               .data(prepend, append, data, size);
    }


private:
    std::ostream& m_os;
};


int main() {
    char data[500]{};
    std::iota(std::begin(data), std::end(data), 0);

    auto data_begin = 0;
    auto data_end   = 500;
    auto line_size  = 16;

    assert(data_end >= data_begin);

    auto data_aligned_begin = data_begin / line_size * line_size;
    auto data_aligned_end   = (data_end / line_size + 1) * line_size;

    for (auto i = data_aligned_begin; i < data_aligned_end; i += line_size) {
        auto row_aligned_begin = i;
        auto row_aligned_end   = row_aligned_begin + line_size;

        auto row_begin =
            row_aligned_begin < data_begin ? data_begin : row_aligned_begin;
        auto row_end = row_aligned_end > data_end ? data_end : row_aligned_end;

        formatter{std::cout}.line(row_aligned_begin,
                                  row_begin - row_aligned_begin,
                                  row_aligned_end - row_end,
                                  data + row_begin, row_end - row_begin);

        std::cout << "\n";
    }
}
