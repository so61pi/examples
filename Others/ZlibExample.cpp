#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>
#include <zlib.h>


const std::size_t DEFAULT_ALLOCATE_SIZE = 100;


//
// compress string
//
std::string compress(
        const std::string& input,
        const int level = Z_DEFAULT_COMPRESSION,
        const std::size_t allocateSize = DEFAULT_ALLOCATE_SIZE)
{
    if (!((level == Z_DEFAULT_COMPRESSION) || (level >= 0 && level <= 9)))
        throw std::invalid_argument("Invalid compression level.");

    std::vector<char> output{};
    output.resize(allocateSize);

    z_stream zstream{};
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;

    zstream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input.data()));
    zstream.avail_in = input.size();

    zstream.next_out = reinterpret_cast<Bytef *>(output.data());
    zstream.avail_out = output.size();

    if (deflateInit(&zstream, Z_DEFAULT_COMPRESSION) != Z_OK)
        throw std::runtime_error("Cannot compress data.");

    while (true) {
        auto status = deflate(&zstream, Z_FINISH);
        if (status == Z_OK || status == Z_BUF_ERROR) {
            // need more output space
            auto lastSize = output.size();
            output.resize(lastSize + allocateSize);
            zstream.next_out = reinterpret_cast<Bytef *>(&output.at(lastSize));
            zstream.avail_out = output.size() - lastSize;
        }
        else if (status == Z_STREAM_END) {
            // done
            // remove unused space
            output.resize(output.size() - zstream.avail_out);
            break;
        }
        else {
            throw std::runtime_error("Cannot compress data.");
        }
    };

    deflateEnd(&zstream);

    return std::string(output.begin(), output.end());
}


//
// decompress string
//
std::string decompress(
        const std::string& input,
        const std::size_t allocateSize = DEFAULT_ALLOCATE_SIZE)
{
    std::vector<char> output{};
    output.resize(allocateSize);

    z_stream zstream{};
    zstream.zalloc = Z_NULL;
    zstream.zfree = Z_NULL;
    zstream.opaque = Z_NULL;

    zstream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(input.data()));
    zstream.avail_in = input.size();

    zstream.next_out = reinterpret_cast<Bytef *>(output.data());
    zstream.avail_out = output.size();

    if (inflateInit(&zstream) != Z_OK)
        throw std::runtime_error("Cannot decompress data.");

    while (true) {
        auto status = inflate(&zstream, Z_FINISH);
        if (status == Z_OK || status == Z_BUF_ERROR) {
            // need more output space
            auto lastSize = output.size();
            output.resize(lastSize + allocateSize);
            zstream.next_out = reinterpret_cast<Bytef *>(&output.at(lastSize));
            zstream.avail_out = output.size() - lastSize;
        }
        else if (status == Z_STREAM_END) {
            // done
            // remove unused space
            output.resize(output.size() - zstream.avail_out);
            break;
        }
        else {
            throw std::runtime_error("Cannot decompress data.");
        }
    }

    inflateEnd(&zstream);

    return std::string(output.begin(), output.end());
}



int main() {
    try {
        std::string original = "1234567890 abcdefghijklmnopqrstuvwxyz!";

        auto compressed = compress(original);
        auto decompressed = decompress(compressed);

        std::cout << original << "\n";
        std::cout << decompressed << "\n";
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }
}
