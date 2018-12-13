#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

#include <ifaddrs.h>
#include <linux/if_link.h>
#include <signal.h>
#include <unistd.h>


static bool g_running;


void handle_signal(int /* signal */) { g_running = false; }


void setup_signal_handler() {
    struct sigaction sa;
    sa.sa_handler = &handle_signal;
    sa.sa_flags   = SA_RESTART;
    sigfillset(&sa.sa_mask);

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
}


void usage(std::ostream& os, char const* program) {
    os << "Usage : " << program << " <interface>\n\n";
}


void print_current_time(std::ostream& os) {
    auto    current_time = std::time(nullptr);
    std::tm local_time{};
    localtime_r(&current_time,
                &local_time);

    os << std::put_time(&local_time, "[%Y %m %d - %H:%M:%S]") << "\n";
}


int main(int argc, char* argv[]) {
    std::setbuf(stdout, nullptr);

    if (argc == 1) {
        usage(std::cout, argv[0]);
        return EXIT_SUCCESS;
    }

    auto interface = std::string{argv[1]};

    g_running = true;
    setup_signal_handler();

    try {
        rtnl_link_stats lastStats = {};
        while (g_running) {
            ifaddrs* ifaddr = nullptr;
            if (getifaddrs(&ifaddr) == -1)
                throw std::runtime_error{std::strerror(errno)};

            for (auto ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
                if (!ifa->ifa_addr) continue;

                auto yes = (ifa->ifa_addr->sa_family == AF_PACKET)
                           && (ifa->ifa_data != nullptr)
                           && (ifa->ifa_name == interface);
                if (yes) {
                    auto stats =
                        reinterpret_cast<rtnl_link_stats*>(ifa->ifa_data);
                    auto tx_delta = stats->tx_packets - lastStats.tx_packets;

                    print_current_time(std::cout);
                    std::cout << "TXed    : " << stats->tx_packets << '\n'
                              << "Delta   : " << tx_delta << '\n'
                              << "Dropped : " << stats->tx_dropped << "\n\n";

                    lastStats = *stats;
                }
            }
            freeifaddrs(ifaddr);

            sleep(1);
        }
    } catch (std::exception const& e) {
        std::cout << "Exception : " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cout << "Exception : Unknown\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
