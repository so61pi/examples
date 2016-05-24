#include <cstdlib>
#include <iostream>


int main() {
    auto user = getenv("USER");
    if (user) {
        std::cout << "current user : " << user << "\n";
    } else {
        std::cout << "cannot get current user name.\n";
    }

    auto http_proxy = getenv("HTTP_PROXY");
    if (!http_proxy) {
        http_proxy = getenv("http_proxy");
    }
    if (http_proxy) {
        std::cout << "http proxy : " << http_proxy << "\n";
    } else {
        std::cout << "environment doesn't have http proxy.\n";
    }
}
