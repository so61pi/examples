#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED


#include <sys/time.h>


static unsigned const PORT = 12345;


inline unsigned long long CurrentTimeUsec() {
    auto tv = timeval();
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}


struct TimeStamp {
    unsigned long long ClientEgressTime;
    unsigned long long ServerIngressTime;
    unsigned long long ServerEgressTime;
    unsigned long long ClientIngressTime;
};


inline unsigned long long ClientToServerTime(TimeStamp const& ts) {
    return ts.ServerIngressTime - ts.ClientEgressTime;
}

inline unsigned long long OnServerTime(TimeStamp const& ts) {
    return ts.ServerEgressTime - ts.ServerIngressTime;
}

inline unsigned long long ServerToClientTime(TimeStamp const& ts) {
    return ts.ClientIngressTime - ts.ServerEgressTime;
}

inline unsigned long long TravelTime(TimeStamp const& ts) {
    return ts.ClientIngressTime - ts.ClientEgressTime;
}


#endif // COMMON_H_INCLUDED
