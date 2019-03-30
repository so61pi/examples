#include "Ping.h"

#include <new>
#include <utility>


Ping::Ping(PingConfig config)
    : mPingConfig{ std::move(config) }
    , mPingObj{ ping_construct(), ping_destroy }
{
    if (!mPingObj) {
        throw std::bad_alloc{};
    }

    ping_host_add(mPingObj, mDestAddr.c_str());
}


PingResult Ping::DoPing() {
    ping_send(mPingObj);
    return {};
}
