#include <algorithm>
#include <array>
#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <exception>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/scope_exit.hpp>

#include <pcap.h>


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


    hex_table_formatter(std::ostream& os)
        : m_os{os} {}


    // format data as hex
    auto hex(std::size_t prepend, std::size_t append, char const* data,
             std::size_t size) const -> this_type const& {
        iosflags_preserver ifp{m_os};

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


//
// ethernet header
//
struct mac_addr : std::array<std::uint8_t, 6> {};

struct ether_header {
    mac_addr        dst;
    mac_addr        src;
    std::uint16_t   type;
};


auto parse_ether_header(std::uint8_t const* data) noexcept -> ether_header {
    ether_header hdr;

    std::copy(data, data + 6, hdr.dst.begin());
    std::copy(data + 6, data + 12, hdr.src.begin());
    hdr.type = (data[12] << 8) + data[13];

    return hdr;
}


auto header_length(ether_header const&) noexcept -> std::size_t { return 14; }


//
// ipv4 address
//
struct ipv4_addr : std::array<std::uint8_t, 4> {};

struct ipv4_header {
    std::uint8_t    version;
    std::uint8_t    ihl;
    std::uint8_t    dscp;
    std::uint8_t    ecn;
    std::uint16_t   total_length;
    std::uint16_t   identification;
    std::uint8_t    flags;
    std::uint16_t   fragment_offset;
    std::uint8_t    time_to_live;
    std::uint8_t    protocol;
    std::uint8_t    header_checksum;
    ipv4_addr       src;
    ipv4_addr       dst;
    std::uint32_t   options;
};


auto parse_ipv4_header(std::uint8_t const* data) noexcept -> ipv4_header {
    ipv4_header hdr;
    auto i           = 0;
    hdr.version      = (data[i] & 0xF0) >> 4;
    hdr.ihl          = data[i] & 0x0F;
    hdr.dscp         = (data[i + 1] & 0b1111'1100) >> 2;
    hdr.ecn          = data[i + 1] & 0b11;
    hdr.total_length = (data[i + 2] << 8) + data[i + 3];

    i += 4;
    hdr.identification = (data[i] << 8) + data[i + 1];
    hdr.flags = (data[i + 2] & 0b1110'0000) >> 5;
    hdr.fragment_offset =
        ((data[i + 2] << 8) + data[i + 3]) & 0b0001'1111'1111'1111;

    i += 4;
    hdr.time_to_live    = data[i];
    hdr.protocol        = data[i + 1];
    hdr.header_checksum = (data[i + 2] << 8) + data[i + 3];

    i += 4;
    std::copy(data + i, data + i + 4, hdr.src.begin());

    i += 4;
    std::copy(data + i, data + i + 4, hdr.dst.begin());

    if (hdr.ihl == 5) {
        i += 4;
        hdr.options = (data[i] << 24) + (data[i + 1] << 16) + (data[i + 2] << 8)
                      + data[i + 3];
    }

    return hdr;
}


auto header_length(ipv4_header const& hdr) noexcept -> std::size_t {
    return hdr.ihl * 4;
}


//
// tcp header
//
struct tcp_header {
    std::uint16_t   src_port;
    std::uint16_t   dst_port;
    std::uint32_t   sequence;
    std::uint32_t   acknowledgment;
    std::uint8_t    data_offset;
    std::uint16_t   flags;
    std::uint16_t   window_size;
    std::uint16_t   checksum;
    std::uint16_t   urgent;
    std::uint32_t   options[10];
};


auto parse_tcp_header(std::uint8_t const* data) noexcept -> tcp_header {
    tcp_header hdr;
    auto i       = 0;
    hdr.src_port = (data[i] << 8) + data[i + 1];
    hdr.dst_port = (data[i + 2] << 8) + data[i + 3];

    i += 4;
    hdr.sequence = (data[i] << 24) + (data[i + 1] << 16) + (data[i + 2] << 8)
                   + data[i + 3];

    i += 4;
    hdr.acknowledgment = (data[i] << 24) + (data[i + 1] << 16)
                         + (data[i + 2] << 8) + data[i + 3];

    i += 4;
    hdr.data_offset = (data[i] & 0xF0) >> 4;
    hdr.flags       = (data[i] & 1) << 8 | data[i + 1];
    hdr.window_size = (data[i + 2] << 8) + data[i + 3];

    assert(5 <= hdr.data_offset && hdr.data_offset <= 15);

    i += 4;
    hdr.checksum = (data[i] << 8) + data[i + 1];
    hdr.urgent   = (data[i + 2] << 8) + data[i + 3];

    for (auto j = 5; j < hdr.data_offset; ++j, i += 4) {
        hdr.options[j - 5] = (data[i] << 24) + (data[i + 1] << 16)
                             + (data[i + 2] << 8) + data[i + 3];
        ;
    }

    return hdr;
}


auto header_length(tcp_header const& hdr) noexcept -> std::size_t {
    return hdr.data_offset * 4;
}


//
// printer
//
auto operator<<(std::ostream& os, mac_addr const& addr) -> std::ostream & {
    iosflags_preserver ifp{os};

    os << std::hex;
    for (auto i = 0; i < 5; ++i) {
        os << static_cast<unsigned int>(addr[i]) << ":";
    }
    os << static_cast<unsigned int>(addr[5]);
    return os;
}


auto operator<<(std::ostream& os, ipv4_addr const& addr) -> std::ostream & {
    for (auto i = 0; i < 3; ++i) {
        os << static_cast<unsigned int>(addr[i]) << ".";
    }
    os << static_cast<unsigned int>(addr[3]);
    return os;
}


//
// packet handler
//
void packet_handler(u_char* user, pcap_pkthdr const* pkt_header,
                    u_char const* pkt_data) {

    auto find_http_header_end = [](char const* first, char const* last) -> char const* {
        assert(first <= last);

        for (; !(last - first < 4); ++first) {
            if (first[0] == '\r' && first[1] == '\n'
                && first[2] == '\r' && first[3] == '\n') {
                return first;
            }
        }

        return last;
    };

    auto p = reinterpret_cast<pcap_t*>(user);
    try {
        auto left_length = pkt_header->caplen;
        if (left_length < 14) return;
        auto ether_hdr = parse_ether_header(pkt_data);

        left_length -= header_length(ether_hdr);
        if (left_length < 5 * 4) return;
        auto ip_hdr = parse_ipv4_header(pkt_data + header_length(ether_hdr));

        left_length -= header_length(ip_hdr);
        if (left_length < 5 * 4) return;
        auto tcp_hdr = parse_tcp_header(pkt_data + header_length(ether_hdr)
                                        + header_length(ip_hdr));

        left_length -= header_length(tcp_hdr);

        iosflags_preserver ifp{std::cout};
        std::cout << "==================================================\n"
                  << "packet length     : " << pkt_header->len      << '\n'
                  << "captured length   : " << pkt_header->caplen   << '\n'
                  << "src IP            : " << ip_hdr.src           << '\n'
                  << "src port          : " << tcp_hdr.src_port     << '\n'
                  << "src MAC           : " << ether_hdr.src        << '\n'
                  << "dst IP            : " << ip_hdr.dst           << '\n'
                  << "dst port          : " << tcp_hdr.dst_port     << '\n'
                  << "dst MAC           : " << ether_hdr.dst        << '\n'
                  << "data length       : " << left_length          << '\n';

        hex_table_format(std::cout, reinterpret_cast<char const*>(pkt_data), 0,
                         pkt_header->caplen > 100 ? 100 : pkt_header->caplen);

        std::cout << "\n\n";
    } catch (...) {
        pcap_breakloop(p);
    }
};


int main() {
    try {
        char ebuf[200]{0};

        pcap_if_t* ppifs = nullptr;
        BOOST_SCOPE_EXIT_ALL(&) {
            if (ppifs) pcap_freealldevs(ppifs);
        };

        if (pcap_findalldevs(&ppifs, ebuf) == -1) {
            throw std::runtime_error{ebuf};
        }

        std::vector<pcap_if_t> v;
        for (auto ppif = ppifs; ppif != nullptr; ppif = ppif->next) {
            v.push_back(*ppif);
        }

        for (auto const& pif : v) {
            std::cout << pif.name << '\n' << pif.description << "\n\n";
        }

        auto pif = pcap_open_live(v[2].name, 65535, 1, 1000, ebuf);
        BOOST_SCOPE_EXIT_ALL(&) {
            if (pif) pcap_close(pif);
        };

        if (!pif) throw std::runtime_error{pcap_geterr(pif)};

        bpf_program bp{};
        if (pcap_compile(pif, &bp, "(ip proto \\tcp) and (dst port 80 or 443)", 0, 0xFF)
            == -1) {
            throw std::runtime_error{pcap_geterr(pif)};
        }

        if (pcap_setfilter(pif, &bp) == -1) {
            throw std::runtime_error{pcap_geterr(pif)};
        }

        pcap_loop(pif, -1, packet_handler, reinterpret_cast<u_char*>(pif));
    } catch (std::exception const& e) {
        std::cout << "exception : " << e.what() << "\n";
    } catch (...) {
        std::cout << "unknown exception\n";
    }
}
