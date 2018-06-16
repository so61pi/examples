#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/scope_exit.hpp>
#include <zlib.h>


namespace zlib {
    std::size_t const CONFIG_DEFAULT_ALLOCATE_SIZE = 100;


    std::string compress(std::string const& input, int const level = Z_DEFAULT_COMPRESSION, std::size_t const allocateSize = CONFIG_DEFAULT_ALLOCATE_SIZE) {
        if (!((level == Z_DEFAULT_COMPRESSION) || (0 <= level && level <= 9))) {
            throw std::invalid_argument("Invalid compression level.");
        }

        std::vector<char> output{};
        output.resize(allocateSize);

        z_stream zstream{};
        zstream.zalloc = Z_NULL;
        zstream.zfree = Z_NULL;
        zstream.opaque = Z_NULL;

        zstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
        zstream.avail_in = input.size();

        zstream.next_out = reinterpret_cast<Bytef*>(output.data());
        zstream.avail_out = output.size();

        if (deflateInit(&zstream, Z_DEFAULT_COMPRESSION) != Z_OK) {
            throw std::runtime_error("Cannot compress data.");
        }

        BOOST_SCOPE_EXIT_ALL(&) { deflateEnd(&zstream); };

        while (true) {
            auto const status = deflate(&zstream, Z_FINISH);
            if (status == Z_OK || status == Z_BUF_ERROR) {
                // need more output space
                auto const lastSize = output.size();
                output.resize(lastSize + allocateSize);
                zstream.next_out = reinterpret_cast<Bytef*>(&output.at(lastSize));
                zstream.avail_out = output.size() - lastSize;
            } else if (status == Z_STREAM_END) {
                // done, remove unused space
                output.resize(output.size() - zstream.avail_out);
                break;
            } else {
                throw std::runtime_error("Cannot compress data.");
            }
        }

        return std::string(output.begin(), output.end());
    }


    //
    // decompress string
    //
    std::string decompress(std::string const& input, std::size_t const allocateSize = CONFIG_DEFAULT_ALLOCATE_SIZE) {
        std::vector<char> output{};
        output.resize(allocateSize);

        z_stream zstream{};
        zstream.zalloc = Z_NULL;
        zstream.zfree = Z_NULL;
        zstream.opaque = Z_NULL;

        zstream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
        zstream.avail_in = input.size();

        zstream.next_out = reinterpret_cast<Bytef*>(output.data());
        zstream.avail_out = output.size();

        if (inflateInit(&zstream) != Z_OK) {
            throw std::runtime_error("Cannot decompress data.");
        }

        BOOST_SCOPE_EXIT_ALL(&) { inflateEnd(&zstream); };

        while (true) {
            auto const status = inflate(&zstream, Z_FINISH);
            if (status == Z_OK || status == Z_BUF_ERROR) {
                // need more output space
                auto const lastSize = output.size();
                output.resize(lastSize + allocateSize);
                zstream.next_out = reinterpret_cast<Bytef*>(&output.at(lastSize));
                zstream.avail_out = output.size() - lastSize;
            } else if (status == Z_STREAM_END) {
                // done, remove unused space
                output.resize(output.size() - zstream.avail_out);
                break;
            } else {
                throw std::runtime_error("Cannot decompress data.");
            }
        }

        return std::string(output.begin(), output.end());
    }
}


int main() {
    try {
        auto const original = std::string{ "1234567890 abcdefghijklmnopqrstuvwxyz!" };
        auto const compressed = zlib::compress(original);
        auto const decompressed = zlib::decompress(compressed);

        std::cout << original << "\n"
                  << decompressed << "\n";
    } catch (std::exception const& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
