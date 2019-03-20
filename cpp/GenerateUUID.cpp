#include <iomanip>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>


int main() {
    // create an uuid generator
    boost::uuids::random_generator gen{};

    // generate 2 UUIDs
    for (auto i : { 0, 0 }) {
        // generate
        auto uuid = gen();

        // print it out
        for (auto& i : uuid)
            std::cout << std::setw(2) << std::setfill('0')
                << std::hex << std::uppercase
                << static_cast<int>(i) << " ";

        std::cout << "\n\n";
    }
}
