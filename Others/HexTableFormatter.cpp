#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>


class iosflags_preserver {
public:
    iosflags_preserver(std::ios& os) : m_os{os} { m_flags = m_os.flags(); }

    ~iosflags_preserver() {
        try {
            m_os.flags(m_flags);
        } catch (...) {
        }
    }

private:
    std::ios& m_os;
    std::ios::fmtflags m_flags;
};


class hex_table_formatter {
public:
    using this_type = hex_table_formatter;


    hex_table_formatter(std::ostream& os, std::size_t block)
        : m_os{os}, m_block{block} {}


    // format data as hex
    auto hex(std::size_t prepend, std::size_t append, char const* data,
             std::size_t size) const -> this_type const& {
        iosflags_preserver ifp{m_os};

        std::size_t i = 0;
        for (; prepend > 0; --prepend, ++i) {
            if (i % m_block == 0 && i != 0) m_os << ' ';
            m_os << "..";
        }

        for (auto j = 0U; j < size; ++j, ++i) {
            if (i % m_block == 0 && i != 0) m_os << ' ';
            m_os << std::setw(2) << std::hex
                 << static_cast<unsigned int>(data[j] & 0xFF);
        }

        for (; append > 0; --append, ++i) {
            if (i % m_block == 0 && i != 0) m_os << ' ';
            m_os << "..";
        }

        return *this;
    }


    // format data
    auto data(std::size_t prepend, std::size_t append, char const* data,
              std::size_t size) const -> this_type const& {
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
    auto address(std::size_t addr) const -> this_type const& {
        iosflags_preserver ifp{m_os};

        m_os << std::setw(8) << std::hex << addr;
        return *this;
    }


    // address - hex fence
    auto ahfence() const -> this_type const& {
        m_os << "  |  ";
        return *this;
    }


    // hex - data fence
    auto hdfence() const -> this_type const& {
        return ahfence();
    }


    //
    auto line(std::size_t addr, std::size_t prepend, std::size_t append,
              char const* data, std::size_t size) const -> this_type const& {
        return  address(addr)
               .ahfence()
               .hex(prepend, append, data, size)
               .hdfence()
               .data(prepend, append, data, size);
    }


private:
    std::ostream& m_os;
    std::size_t m_block;
};


void hex_table_format(std::ostream& os, char const* data, std::size_t begin_off,
                      std::size_t end_off, std::size_t line_size = 16) {
    assert(end_off >= begin_off);

    auto data_aligned_begin = begin_off / line_size * line_size;
    auto data_aligned_end   = (end_off / line_size + 1) * line_size;

    for (auto i = data_aligned_begin; i < data_aligned_end; i += line_size) {
        auto row_aligned_begin = i;
        auto row_aligned_end   = row_aligned_begin + line_size;

        auto row_begin =
            row_aligned_begin < begin_off ? begin_off : row_aligned_begin;
        auto row_end = row_aligned_end > end_off ? end_off : row_aligned_end;

        hex_table_formatter{os}.line(row_aligned_begin,
                                     row_begin - row_aligned_begin,
                                     row_aligned_end - row_end,
                                     data + row_begin, row_end - row_begin);
        os << "\n";
    }
}


int main() {
    char data[500]{};
    std::iota(std::begin(data), std::end(data), 0);

    hex_table_format(std::cout, data, 0, 500);
}
