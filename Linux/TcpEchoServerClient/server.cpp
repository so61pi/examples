#include <cerrno>
#include <cstdio>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT        12345
#define MAXBUF      1024


int main() {
    auto const sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return errno;
    }

    int option = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
        std::perror("setsockopt");
        return errno;
    }

    auto saddr            = sockaddr_in();
    saddr.sin_family      = AF_INET;
    saddr.sin_port        = htons(PORT);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, reinterpret_cast<sockaddr*>(&saddr), sizeof(saddr)) < 0) {
        std::perror("bind");
        return errno;
    }

    if (listen(sockfd, 20) < 0) {
        std::perror("listen");
        return errno;
    }

    while (true) {
        auto caddr = sockaddr_in();
        unsigned caddrlen = sizeof(caddr);

        int clientfd = accept(sockfd, reinterpret_cast<sockaddr*>(&caddr), &caddrlen);
        if (clientfd < 0) {
            std::perror("accept");
            return errno;
        }
        std::cout << inet_ntoa(caddr.sin_addr) << ':' << ntohs(caddr.sin_port) << " connected" << std::endl;

        char buffer[MAXBUF] = {};
        auto const rsize = recv(clientfd, buffer, MAXBUF, 0);
        if (rsize < 0) {
            std::perror("recv");
            return errno;
        }

        if (send(clientfd, buffer, rsize, 0) < 0) {
            std::perror("send");
            return errno;
        }

        close(clientfd);
    }

    close(sockfd);
    return 0;
}
