#ifndef PING_H
#define PING_H

#include <string>
#include <cinttypes>
#include <memory>
#include <vector>

#include <oping.h>

using pingobj_pointer = std::unique_ptr<pingobj_t, decltype(&ping_destroy)>;

struct PingConfig {
    std::string mDestAddr = "127.0.0.1";
    double mTimeout = PING_DEF_TIMEOUT;
    std::uint8_t mTTL = PING_DEF_TTL;
    std::string mNetDev;
};

struct PingResult {};

// std::vector<PingResult>

class Ping {
public:
    explicit Ping(PingConfig config);
    PingResult DoPing();

private:
    PingConfig const mPingConfig;
    pingobj_pointer mPingObj;
};

#endif // PING_H
