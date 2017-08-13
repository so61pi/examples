#include <algorithm>
#include <cinttypes>
#include <csignal>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <fcntl.h>
#include <netinet/ip.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <boost/scope_exit.hpp>


using chunk_t = std::vector<std::uint8_t>;
using data_t = std::list<chunk_t>;
using data_container_t = std::unordered_map<int, data_t>;


int setnonblocking(int sock_fd) {
    int flags = fcntl(sock_fd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(sock_fd, F_SETFL, flags) == -1) {
        return -1;
    }

    return 0;
}


/**
 * @brief Read all available data of socket.
 * @return
 *      1 on success.
 *      0 on end of file (socket is closed by other end).
 *     -1 on error.
 */
int nonblockread(int sockfd, std::vector<std::uint8_t>& buffer) {
    char buf[100];
    while (true) {
        auto bytes = read(sockfd, buf, sizeof(buf));
        if (bytes < 0) {
            if      (errno == EINTR)                            continue;
            else if (errno == EAGAIN || errno == EWOULDBLOCK)   return 1;
            else                                                return -1;
        } else if (bytes == 0) {
            return 0;
        } else {
            std::copy(buf, buf + bytes, std::back_inserter(buffer));
        }
    }
}


int nonblockread(int sockfd, std::vector<std::uint8_t>& buffer, std::size_t size) {
    char buf[100];
    while (size > 0) {
        auto bytes = read(sockfd, buf, std::min(size, sizeof(buf)));
        if (bytes < 0) {
            if      (errno == EINTR)                            continue;
            else if (errno == EAGAIN || errno == EWOULDBLOCK)   return 1;
            else                                                return -1;
        } else if (bytes == 0) {
            return 0;
        } else {
            std::copy(buf, buf + bytes, std::back_inserter(buffer));
            size -= bytes;
        }
    }
    return 1;
}


/**
 * @brief Read all available data of socket.
 * @return
 *      1 on success.
 *      0 on end of file (socket is closed by other end).
 *     -1 on error.
 *
 * @note Used (*newoutput - output) to detect number of bytes read.
 */
int nonblockread(int sockfd, std::uint8_t* output, std::size_t size, std::uint8_t** newoutput) {
    char buf[100];
    while (size > 0) {
        auto bytes = read(sockfd, buf, std::min(size, sizeof(buf)));
        if (bytes < 0) {
            if      (errno == EINTR)                            continue;
            else if (errno == EAGAIN || errno == EWOULDBLOCK)   return 1;
            else                                                return -1;
        } else if (bytes == 0) {
            return 0;
        } else {
            std::copy(buf, buf + bytes, output);
            size       -= bytes;
            output     += bytes;
            *newoutput  = output;
        }
    }
    return 1;
}


void display(std::ostream& os, int fd, std::vector<std::uint8_t> const& data) {
    os << '[' << fd << "] received\n\t";
    for (auto d : data) os << d << ' ';
    os << '\n' << std::endl;
}


int add_data_chunk(data_container_t& container, int efd, int fd, chunk_t chunk) {
    std::cout << "[info] prepare echo data for " << fd << std::endl;

    if (chunk.empty()) return 0;

    epoll_event ev = {};
    ev.events      = EPOLLIN | EPOLLOUT;
    ev.data.fd     = fd;
    if (epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev) < 0) {
        return -1;
    }

    container[fd].push_back(std::move(chunk));
    return 0;
}


int send_chunk(int fd, chunk_t& data) {
    auto const* bytes = data.data();
    auto size = data.size();

    while (size > 0) {
        auto const b = write(fd, bytes, size);
        if (b >= 0) {
            size -= b;
            bytes += b;
            continue;
        } else {
            if (errno == EINTR) {
                continue;
            } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            } else {
                return -1;
            }
        }
    }

    data = chunk_t(data.begin() + (bytes - data.data()), data.end());
    return 0;
}


void remove_data(data_container_t& container, int fd) {
    container.erase(fd);
}


int send_data(data_container_t& container, int efd, int fd) {
    auto const it = container.find(fd);
    if (it == container.end()) return 0;

    data_t& data = it->second;

    std::cout << "[info] send echo data of " << fd << std::endl;

    int status = 0;
    for (auto& chunk : data) {
        status = send_chunk(fd, chunk);
        if (status < 0) break;
    }

    data.remove_if([](chunk_t const& chunk) { return chunk.empty(); });

    if (data.empty()) {
        remove_data(container, fd);

        epoll_event ev = {};
        ev.events      = EPOLLIN;
        ev.data.fd     = fd;
        if (epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev) < 0) {
            return -1;
        }
    } else {
        epoll_event ev = {};
        ev.events      = EPOLLIN | EPOLLOUT;
        ev.data.fd     = fd;
        if (epoll_ctl(efd, EPOLL_CTL_MOD, fd, &ev) < 0) {
            remove_data(container, fd);
            return -1;
        }
    }

    return status;
}


int main() {
    constexpr int MAXCONN        = 10;
    constexpr std::uint16_t PORT = 12344;

    // Make sure that write to a closed socket return EPIPE instead of raising SIGPIPE.
    std::signal(SIGPIPE, SIG_IGN);

    auto listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        std::cerr << "[error] socket : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { close(listenfd); };

    int yes;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        std::cerr << "[error] setsockopt : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    sockaddr_in serveraddr     = {};
    serveraddr.sin_family      = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port        = htons(PORT);
    if (bind(listenfd, reinterpret_cast<struct sockaddr *>(&serveraddr), sizeof(serveraddr)) < 0) {
        std::cerr << "[error] bind : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    if (setnonblocking(listenfd) < 0) {
        std::cerr << "[error] setnonblocking : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    if (listen(listenfd, MAXCONN) < 0) {
        std::cerr << "[error] listen : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    auto epfd = epoll_create(1); // size parameter is ignored
    if (epfd < 0) {
        std::cerr << "[error] epoll_create : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { close(epfd); };

    epoll_event ev = {};
    ev.events      = EPOLLIN;
    ev.data.fd     = listenfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) < 0) {
        std::cerr << "[error] epoll_ctl : " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "[info] listening on " << PORT << std::endl;

    data_container_t data_container;
    while (true) {
        epoll_event events[MAXCONN] = {};
        auto ready = epoll_wait(epfd, events, MAXCONN, -1);

        if (ready < 0) {
            if (errno == EINTR) continue;
            std::cerr << "[error] epoll_wait : " << std::strerror(errno) << std::endl;
            break;
        }

        for (auto i = 0; i < ready; ++i) {
            auto const es = events[i].events;
            auto const fd = events[i].data.fd;

            if (es & EPOLLIN) {
                if (fd == listenfd) {
                    auto newfd = accept(listenfd, nullptr, nullptr);
                    if (newfd < 0) {
                        std::cerr << "[error] accept : " << std::strerror(errno) << std::endl;
                        continue;
                    }

                    if (setnonblocking(newfd) < 0) {
                        std::cerr << "[error] setnonblocking : " << std::strerror(errno) << std::endl;
                        close(newfd);
                        continue;
                    }

                    epoll_event ev = {};
                    ev.events      = EPOLLIN;
                    ev.data.fd     = newfd;
                    if (epoll_ctl(epfd, EPOLL_CTL_ADD, newfd, &ev) < 0) {
                        std::cerr << "[error] epoll_ctl : " << std::strerror(errno) << std::endl;
                        close(newfd);
                        continue;
                    }

                    std::cout << "[info] new connection accepted : " << newfd << std::endl;
                } else {
                    std::vector<std::uint8_t> buffer;
                    auto status = nonblockread(fd, buffer);
                    if (status < 0) {
                        std::cerr << "[error] nonblockread : " << std::strerror(errno) << std::endl;
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                        remove_data(data_container, fd);
                        close(fd);
                    } else if (status == 0) {
                        std::cerr << "[error] nonblockread : " << std::strerror(errno) << std::endl;
                        display(std::cout, fd, buffer);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, nullptr);
                        remove_data(data_container, fd);
                        close(fd);
                    } else {
                        display(std::cout, fd, buffer);
                        add_data_chunk(data_container, epfd, fd, std::move(buffer));
                    }
                }
            }

            if (es & EPOLLOUT) {
                send_data(data_container, epfd, fd);
            }

            if (es & (EPOLLHUP | EPOLLERR)) {
                std::cout << "[server] close connection : " << fd << std::endl;
                epoll_ctl(epfd, EPOLL_CTL_ADD, fd, nullptr);
                remove_data(data_container, fd);
                close(fd);
            }
        }
    }

    std::cout << "[server] stopped" << std::endl;
}
