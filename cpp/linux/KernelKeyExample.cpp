#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>

#include <boost/scope_exit.hpp>

extern "C" { // to avoid C++ name mangle for this header
#include <keyutils.h>
}


// g++ -std=c++14 -Wall -Wextra -pedantic main.cpp -o main.out -lkeyutils


template <typename T>
void check_error(T&& val) {
    if (val < 0) {
        throw std::runtime_error{ strerror(errno) };
    }
}


int main() {
    try {
        // add new keyring
        auto keyring_serial = add_key("keyring", "myapp:keyring", nullptr, 0, KEY_SPEC_SESSION_KEYRING);
        BOOST_SCOPE_EXIT_ALL(&) {
            // clear content of the keyring
            keyctl_clear(keyring_serial);
        };
        check_error(keyring_serial);

        // add a key to keyring
        char const key_data[] = "this is the data of my key";
        auto key_serial = add_key("user", "myapp:key", reinterpret_cast<void*>(const_cast<char*>(key_data)), sizeof(key_data), keyring_serial);
        check_error(key_serial);

        // read it
        // similar to
        //      keyctl(KEYCTL_READ, key_serial, buffer, buffer_size);
        char const* read_key_data_buffer = nullptr;
        BOOST_SCOPE_EXIT_ALL(&) {
            free(reinterpret_cast<void*>(const_cast<char*>(read_key_data_buffer)));
        };
        check_error(keyctl_read_alloc(key_serial, reinterpret_cast<void**>(const_cast<char**>(&read_key_data_buffer))));
        std::cout << "myapp:key data : " << read_key_data_buffer << "\n";

        // find key
        auto found_key_serial = request_key("user", "myapp:key", nullptr, keyring_serial);
        check_error(found_key_serial);
        std::cout << "myapp:key found\n";
    } catch (std::exception const& e) {
        std::cerr << "exception : " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "exception : unknown" << std::endl;
    }
}
