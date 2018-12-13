#include <cerrno>
#include <cstdio>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include "common.h"


int ServeClient(int clientfd) {
    while (true) {
        TimeStamp ts = {0, 0, 0, 0};
        auto const rsize = recv(clientfd, reinterpret_cast<void*>(&ts), sizeof(ts), 0);
        if (rsize < 0) {
            std::perror("recv");
            return -1;
        } else if (rsize == 0) {
            return 0;
        }

        ts.ServerIngressTime = ts.ServerEgressTime = CurrentTimeUsec();
        if (send(clientfd, reinterpret_cast<void*>(&ts), sizeof(ts), 0) < 0) {
            std::perror("send");
            return -1;
        }
    }
}


int main() {
    auto const sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::perror("socket");
        return -1;
    }

    int option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        std::perror("setsockopt");
        return -1;
    }

    auto saddr            = sockaddr_in();
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0) {
        std::perror("bind");
        return -1;
    }

    if (listen(sockfd, 20) < 0) {
        std::perror("listen");
        return -1;
    }

    auto caddr = sockaddr_in();
    unsigned caddrlen = sizeof(caddr);

    while (true) {
        int clientfd = accept(sockfd, reinterpret_cast<sockaddr*>(&caddr), &caddrlen);
        if (clientfd < 0) {
            std::perror("accept");
            return -1;
        }
        std::cout << inet_ntoa(caddr.sin_addr) << ':' << ntohs(caddr.sin_port) << " connected" << std::endl;

        if (ServeClient(clientfd) < 0) {
            return -1;
        }

        close(clientfd);
    }
    close(sockfd);
    return 0;
}
