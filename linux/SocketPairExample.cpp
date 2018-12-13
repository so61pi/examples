#include <cstdlib>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


int main() {
    int sv[2];
    if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0, sv) < 0) {
        std::cout << strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    auto const client = sv[0];
    auto const server = sv[1];

    /// Send data to server.
    char const* const data = "Hello World!";
    auto const datasize = strlen(data);
    auto const sent = write(client, data, datasize);

    {
        /// Server receives and echo back data.
        char tmp[100] = {};
        auto const recved = read(server, tmp, datasize);
        write(server, tmp, recved);
    }

    /// Client receives echoed data.
    char tmp[100] = {};
    read(client, tmp, datasize);

    std::cout << tmp << std::endl;

    close(client);
    close(server);
    return EXIT_SUCCESS;
}
