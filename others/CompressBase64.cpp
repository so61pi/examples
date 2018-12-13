#include <cstdint>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>

#include <zlib.h>


using std::begin;
using std::end;


namespace base64 {
    namespace bi = boost::archive::iterators;
    using base64_encode_it = bi::base64_from_binary<bi::transform_width<std::string::const_iterator, 6, 8>>;
    using base64_decode_it = bi::transform_width<bi::binary_from_base64<std::string::const_iterator>, 8, 6>;

    std::string encode(std::string const& data) {
        std::string encoded(base64_encode_it(begin(data)), base64_encode_it(end(data)));
        encoded.append((3 - data.size() % 3) % 3, '=');
        return encoded;
    }

    std::string decode(std::string const& data) {
        return std::string(base64_decode_it(begin(data)), base64_decode_it(end(data)));
    }
}


namespace zlib {
    namespace io = boost::iostreams;

    std::string compress(std::string const& data) {
        std::stringstream compressed, decompressed;
        decompressed << data;

        io::filtering_streambuf<io::input> out;
        out.push(io::zlib_compressor());
        out.push(decompressed);

        io::copy(out, compressed);
        return compressed.str();
    }

    std::string decompress(std::string const& data) {
        std::stringstream compressed, decompressed;
        compressed << data;

        io::filtering_streambuf<io::input> in;
        in.push(io::zlib_decompressor());       
        in.push(compressed); 

        io::copy(in, decompressed);
        return decompressed.str();
    }
}


std::string compress_to_base64(std::string const& data) {
    return base64::encode(zlib::compress(data));
}


std::string decompress_from_base64(std::string const& data) {
    return zlib::decompress(base64::decode(data));
}


int main() {
    auto const data = "12345678900";
    auto const encoded = compress_to_base64(data);
    auto const decoded = decompress_from_base64(encoded);

    std::cout << encoded << '\n'
              << decoded << '\n';
}
