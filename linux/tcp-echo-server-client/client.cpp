#include <cerrno>
#include <cstdio>
#include <iostream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT            12345
#define SERVER_ADDR     "127.0.0.1"
#define MAXBUF          1024


int main() {
    auto const sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
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

    char const sbuffer[] = "hello";
    if (send(sockfd, sbuffer, sizeof(sbuffer), 0) < 0) {
        std::perror("send");
        return errno;
    }

    char rbuffer[MAXBUF] = {0};
    if (recv(sockfd, rbuffer, sizeof(sbuffer), 0) < 0) {
        std::perror("recv");
        return errno;
    }
    std::cout << rbuffer << std::endl;

    close(sockfd);
    return 0;
}
