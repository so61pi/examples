#include <algorithm>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>

#include <boost/scope_exit.hpp>

#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netdb.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <sys/capability.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>


static constexpr auto ETH_HDRLEN = sizeof(ether_header);
static constexpr auto IP4_HDRLEN = sizeof(iphdr);
static constexpr auto IP6_HDRLEN = sizeof(ip6_hdr);
static constexpr auto UDP_HDRLEN = sizeof(udphdr);


// Computing the internet checksum (RFC 1071).
auto checksum(std::uint8_t const* data, std::size_t size) -> std::uint16_t {
    std::uint32_t sum = 0;

    while (size > 1) {
        /*  This is the inner loop */
        sum += *reinterpret_cast<std::uint16_t const*>(data);
        size -= 2;
        ++data;
    }

    /*  Add left-over byte, if any */
    if (size > 0) sum += *data;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return static_cast<std::uint16_t>(~sum);
}


auto socket_raw() -> int {
    auto sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) throw std::runtime_error{std::strerror(errno)};
    return sock;
}


auto get_interface_index(int sock, char const* ifname) -> int {
    ifreq req = {};
    strncpy(req.ifr_name, ifname, sizeof(req.ifr_name));
    if (ioctl(sock, SIOCGIFINDEX, &req))
        throw std::runtime_error{std::strerror(errno)};
    return req.ifr_ifindex;
}


void get_local_mac(int sock, char const* ifname, uint8_t* mac) {
    ifreq req = {};
    strncpy(req.ifr_name, ifname, sizeof(req.ifr_name));
    if (ioctl(sock, SIOCGIFHWADDR, &req) < 0)
        throw std::runtime_error{std::strerror(errno)};
    std::copy_n(std::begin(req.ifr_hwaddr.sa_data), ETH_ALEN, mac);
}


auto get_local_ip4(int sock, char const* ifname) -> in_addr {
    ifreq req = {};
    strncpy(req.ifr_name, ifname, sizeof(req.ifr_name));
    if (ioctl(sock, SIOCGIFADDR, &req) < 0)
        throw std::runtime_error{std::strerror(errno)};
    return reinterpret_cast<sockaddr_in*>(&req.ifr_addr)->sin_addr;
}


auto create_dest_sockaddr(std::uint8_t const* dstmac, int ifidx)
    -> sockaddr_ll {
    // When you send packets it is enough to specify sll_family, sll_addr,
    // sll_halen, sll_ifindex. The other fields should be 0. sll_hatype and
    // sll_pkttype are set on received packets for your information. For
    // bind only sll_protocol and sll_ifindex are used.
    sockaddr_ll sockaddr = {};
    sockaddr.sll_family  = PF_PACKET;
    sockaddr.sll_ifindex = ifidx;
    sockaddr.sll_halen   = ETH_ALEN;
    std::copy_n(dstmac, ETH_ALEN, sockaddr.sll_addr);
    return sockaddr;
}


auto construct_ether_header(std::uint8_t* buffer, std::uint8_t const* srcmac,
                            std::uint8_t const* dstmac, std::uint16_t ethertype)
    -> std::size_t {
    auto eh = reinterpret_cast<ether_header*>(buffer);

    std::copy_n(srcmac, ETH_ALEN, eh->ether_shost);
    std::copy_n(dstmac, ETH_ALEN, eh->ether_dhost);
    eh->ether_type = htons(ethertype);
    return sizeof(*eh);
}


auto construct_ip4_header(std::uint8_t* buffer, std::uint8_t proto,
                          in_addr_t srcip, in_addr_t dstip,
                          std::uint16_t dtlen = 0, std::uint8_t tos = 46,
                          std::uint8_t ttl = 100) -> std::size_t {
    static_assert(IP4_HDRLEN == sizeof(iphdr), "Invalid IP4_HDRLEN.");

    constexpr auto header_length_dword = 5U;
    constexpr auto version             = 4;

    auto iph      = reinterpret_cast<iphdr*>(buffer);
    iph->ihl      = header_length_dword;
    iph->version  = version;
    iph->tos      = tos;
    iph->tot_len  = htons(IP4_HDRLEN + dtlen);
    iph->id       = htons(54321);
    iph->ttl      = ttl;
    iph->protocol = proto;
    iph->saddr    = srcip;
    iph->daddr    = dstip;

    return IP4_HDRLEN;
}


auto construct_ip6_header(std::uint8_t* buffer, std::uint8_t proto,
                          in6_addr const& srcip, in6_addr const& dstip,
                          std::uint16_t dtlen = 0, std::uint8_t hoplimit = 255)
    -> std::size_t {
    static_assert(IP6_HDRLEN == sizeof(ip6_hdr), "Invalid IP4_HDRLEN.");

    constexpr auto version = 6;

    auto iph      = reinterpret_cast<ip6_hdr*>(buffer);
    iph->ip6_flow = htonl((version << 28) | (0 << 20) | 0);
    iph->ip6_plen = htons(dtlen);
    iph->ip6_nxt  = proto;
    iph->ip6_hops = hoplimit;
    iph->ip6_src  = srcip;
    iph->ip6_dst  = dstip;

    return IP6_HDRLEN;
}


auto construct_udp_header(std::uint8_t* buffer, std::uint16_t srcport,
                          std::uint16_t dstport, std::uint16_t dtlen = 0)
    -> std::size_t {
    static_assert(UDP_HDRLEN == sizeof(udphdr), "Invalid UDP_HDRLEN.");

    auto header    = reinterpret_cast<udphdr*>(buffer);
    header->source = htons(srcport);
    header->dest   = htons(dstport);
    header->len    = htons(UDP_HDRLEN + dtlen);
    header->check  = 0;

    return UDP_HDRLEN;
}


auto raw_socket_permitted() -> bool {
    if (geteuid() == 0) return true;

#ifdef __linux__
    auto cap = cap_get_proc();
    if (!cap) throw std::runtime_error{std::strerror(errno)};
    BOOST_SCOPE_EXIT_ALL(&) { cap_free(cap); };

    auto dup = cap_dup(cap);
    if (!dup) throw std::runtime_error{std::strerror(errno)};
    BOOST_SCOPE_EXIT_ALL(&) { cap_free(dup); };

    cap_value_t netraw = CAP_NET_RAW;
    if (cap_set_flag(dup, CAP_PERMITTED, 1, &netraw, CAP_SET) < 0)
        throw std::runtime_error{std::strerror(errno)};
    if (cap_set_flag(dup, CAP_EFFECTIVE, 1, &netraw, CAP_SET) < 0)
        throw std::runtime_error{std::strerror(errno)};

    return cap_compare(cap, dup) == 0;
#else
    return false;
#endif
}


auto create_udp4_packet_example(std::uint8_t* buffer) -> std::size_t {
    std::uint8_t const data[] = "0123456789";

    std::uint8_t const srcmac[ETH_ALEN] = {1, 2, 3, 4, 5, 6};
    std::uint8_t const dstmac[ETH_ALEN] = {2, 4, 6, 7, 10, 12};

    in_addr_t const srcip = inet_addr("192.168.0.111");
    in_addr_t const dstip = inet_addr("192.168.0.222");

    std::uint16_t const srcport = 11;
    std::uint16_t const dstport = 11;

    std::size_t consumed = 0;

    auto const ptr_ether_hdr = buffer + consumed;
    consumed += construct_ether_header(ptr_ether_hdr, srcmac, dstmac, ETH_P_IP);

    auto const ptr_ip4_hdr = buffer + consumed;
    consumed += construct_ip4_header(ptr_ip4_hdr, IPPROTO_UDP, srcip, dstip,
                                     UDP_HDRLEN + sizeof(data));

    auto ptr_udp_hdr = buffer + consumed;
    consumed +=
        construct_udp_header(ptr_udp_hdr, srcport, dstport, sizeof(data));

    auto const ptr_udp_data = buffer + consumed;
    memcpy(ptr_udp_data, data, sizeof(data));
    consumed += sizeof(data);

    auto const ptr_pkt_end = buffer + consumed;

    auto const sum =
        checksum(ptr_ip4_hdr,
                 static_cast<std::size_t>(ptr_pkt_end - ptr_ip4_hdr));
    reinterpret_cast<udphdr*>(ptr_udp_hdr)->check = sum;

    return consumed;
}


auto create_udp6_packet_example(std::uint8_t* buffer) -> std::size_t {
    std::uint8_t const data[] = "0123456789";

    std::uint8_t const srcmac[ETH_ALEN] = {1, 2, 3, 4, 5, 6};
    std::uint8_t const dstmac[ETH_ALEN] = {2, 4, 6, 7, 10, 12};

    in6_addr srcip = {};
    inet_pton(AF_INET6, "::ffff:192.168.0.111", &srcip);

    in6_addr dstip = {};
    inet_pton(AF_INET6, "::ffff:192.168.0.222", &dstip);

    std::uint16_t const srcport = 11;
    std::uint16_t const dstport = 11;

    std::size_t consumed = 0;

    auto const ptr_ether_hdr = buffer + consumed;
    consumed +=
        construct_ether_header(ptr_ether_hdr, srcmac, dstmac, ETH_P_IPV6);

    auto const ptr_ip6_hdr = buffer + consumed;
    consumed += construct_ip6_header(ptr_ip6_hdr, IPPROTO_UDP, srcip, dstip,
                                     UDP_HDRLEN + sizeof(data));

    auto ptr_udp_hdr = buffer + consumed;
    consumed +=
        construct_udp_header(ptr_udp_hdr, srcport, dstport, sizeof(data));

    auto const ptr_udp_data = buffer + consumed;
    memcpy(ptr_udp_data, data, sizeof(data));
    consumed += sizeof(data);

    auto const ptr_pkt_end = buffer + consumed;

    auto const sum =
        checksum(ptr_ip6_hdr,
                 static_cast<std::size_t>(ptr_pkt_end - ptr_ip6_hdr));
    reinterpret_cast<udphdr*>(ptr_udp_hdr)->check = sum;

    return consumed;
}


int main() {
    try {
        if (!raw_socket_permitted()) {
            std::cerr << "Doesn't have permission to use raw socket.\n";
            return EXIT_FAILURE;
        }

        auto                      ifname           = "eth0";
        std::uint8_t              dstmac[ETH_ALEN] = {11, 12, 13, 14, 15, 16};
        std::vector<std::uint8_t> buffer(IP_MAXPACKET);

        auto sock = socket_raw();
        BOOST_SCOPE_EXIT_ALL(&) { close(sock); };

        auto ifidx    = get_interface_index(sock, ifname);
        auto pktlen   = create_udp4_packet_example(buffer.data());
        auto sockaddr = create_dest_sockaddr(dstmac, ifidx);

        if (sendto(sock, buffer.data(), pktlen, 0,
                   reinterpret_cast<struct sockaddr*>(&sockaddr),
                   sizeof(sockaddr))
            < 0) {
            throw std::runtime_error{std::strerror(errno)};
        } else {
            std::cout << "Send successfully.\n";
        }
        return EXIT_SUCCESS;
    } catch (std::exception const& e) {
        std::cerr << "Exception : " << e.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Exception : Unknown.\n";
        return EXIT_FAILURE;
    }
}
