#include <iomanip>
#include <iostream>

#include <openssl/sha.h>


std::ostream& PrintHex(std::ostream& os, unsigned char const* data, std::size_t size) {
    for (std::size_t i = 0; i < size; ++i) {
        os << std::setfill('0') << std::setw(2) << std::hex << static_cast<unsigned int>(data[i]) << ' ';
    }
    return os;
}


int main() {
    unsigned char data[] = "so61pi";
    auto const length = sizeof(data) - 1;
    unsigned char hash[SHA_DIGEST_LENGTH]{};
    SHA1(data, length, hash);
    PrintHex(std::cout, hash, sizeof(hash)) << std::endl;

    SHA_CTX c;
    SHA1_Init(&c);
    SHA1_Update(&c, "so", 2);
    SHA1_Update(&c, "61", 2);
    SHA1_Update(&c, "pi", 2);
    SHA1_Final(hash, &c);
    PrintHex(std::cout, hash, sizeof(hash)) << std::endl;
}
