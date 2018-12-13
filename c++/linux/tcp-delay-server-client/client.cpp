#include <cerrno>
#include <cstdio>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "common.h"


std::ostream& Display(std::ostream& os, TimeStamp const& ts) {
    os << "client -> server           = " << ClientToServerTime(ts) << " (us)" << std::endl
       << "          server -> client = " << ServerToClientTime(ts) << " (us)" << std::endl
       << "client -> server -> client = " << TravelTime(ts) << " (us)" << std::endl;
    return os;
}


int main() {
    auto const sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::perror("socket");
        return errno;
    }

    auto dest = sockaddr_in();
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    if (inet_aton("127.0.0.1", reinterpret_cast<in_addr*>(&dest.sin_addr.s_addr)) == 0) {
        std::perror("inet_aton");
        return errno;
    }

    if (connect(sockfd, reinterpret_cast<sockaddr*>(&dest), sizeof(dest)) != 0) {
        std::perror("connect");
        return errno;
    }

    while (true) {
        TimeStamp ts{CurrentTimeUsec(), 0, 0, 0};
        if (send(sockfd, reinterpret_cast<void*>(&ts), sizeof(ts), 0) < 0) {
            std::perror("send");
            return errno;
        }

        if (recv(sockfd, reinterpret_cast<void*>(&ts), sizeof(ts), 0) < 0) {
            std::perror("recv");
            return errno;
        }

        ts.ClientIngressTime = CurrentTimeUsec();
        Display(std::cout, ts) << std::endl;
    }

    close(sockfd);
    return 0;
}
