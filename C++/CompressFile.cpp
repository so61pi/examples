#include <algorithm>
#include <array>
#include <cinttypes>
#include <exception>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>


std::array<char, 8> cut(std::uint64_t num) {
    std::array<char, 8> pieces{};
    for (auto& e : pieces) {
        e = num & 0xFF;
        num >>= 8;
    }
    return pieces;
}


std::uint64_t join(std::array<char, 8> const& pieces) {
    std::uint64_t num{};
    for (auto it = rbegin(pieces), eit = rend(pieces); it != eit; ++it) {
        num <<= 8;
        num |= (*it) & 0xFF;
    }
    return num;
}


class packer {
public:
    explicit packer(std::ostream& os) :
        m_os{ os }
    {}

    auto operator<<(std::istream& is) -> packer& {
        boost::iostreams::filtering_istream in;
        in.set_auto_close(false);
        in.push(boost::iostreams::bzip2_compressor{});
        in.push(is);

        // compressing
        std::stringstream ss;
        std::copy(std::istreambuf_iterator<char>{ in },
                  std::istreambuf_iterator<char>{},
                  std::ostreambuf_iterator<char>{ ss });

        // write length of the compressed file
        auto pieces = cut(ss.tellp());
        m_os.write(pieces.data(), pieces.size());

        // write compressed file
        std::copy(std::istreambuf_iterator<char>{ ss },
                  std::istreambuf_iterator<char>{},
                  std::ostreambuf_iterator<char>{ m_os });

        return *this;
    }

private:
    std::ostream& m_os;
};


class unpacker {
public:
    explicit unpacker(std::istream& is) :
        m_is{ is }
    {}

    auto operator>>(std::ostream& os) -> unpacker& {
        // read len
        std::array<char, 8> pieces{};
        m_is.read(pieces.data(), pieces.size());

        boost::iostreams::filtering_ostream out;
        out.set_auto_close(false);
        out.push(boost::iostreams::bzip2_decompressor{});
        out.push(os);

        // decompress (cannot use std::copy_n)
        std::generate_n(std::ostreambuf_iterator<char>{ out }, join(pieces),
                        [it = std::istreambuf_iterator<char>{ m_is }]() mutable -> char {
                            return *it++;
                        });

        return *this;
    }

private:
    std::istream& m_is;
};


int main() {
    try {
        // compress 2 files
        {
            std::ofstream ofs{ "out.png.bz2", std::ios::out | std::ios::binary };
            packer p{ ofs };

            std::ifstream ifs{ "in.png", std::ios::in | std::ios::binary };
            p << ifs;

            std::ifstream ifs2{ "in.jpg", std::ios::in | std::ios::binary };
            p << ifs2;
        }

        // uncompress them
        {
            std::ifstream ifs{ "out.png.bz2", std::ios::in | std::ios::binary };
            unpacker up{ ifs };

            std::ofstream ofs{ "in.1.png", std::ios::out | std::ios::binary };
            up >> ofs;

            std::ofstream ofs2{ "in.1.jpg", std::ios::out | std::ios::binary };
            up >> ofs2;
        }
    } catch (boost::iostreams::bzip2_error const& e) {
        std::cout << "exception: " << e.what() << ' '
                  << (e.error() == boost::iostreams::bzip2::data_error_magic) << '\n';
    } catch (std::exception const& e) {
        std::cout << "exception: " << e.what() << '\n';
    }
}
