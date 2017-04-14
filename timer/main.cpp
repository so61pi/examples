#include <algorithm>
#include <chrono>
#include <ctime>
#include <exception>
#include <functional>
#include <iostream>
#include <list>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/scope_exit.hpp>

#include <cstdint>
#include <cstdlib>
#include <iostream>

#include <signal.h>
#include <time.h>
#include <unistd.h>


struct Task {
    explicit Task(unsigned long timepoint, unsigned long interval, bool periodic, std::function<void()> handler)
        : runtime(timepoint), interval(interval), periodic(periodic), handler(std::move(handler))
    {}

    unsigned long         runtime;
    unsigned long         interval;
    bool                  periodic;
    std::function<void()> handler;
};


std::ostream& DumpTask(std::ostream& os, Task const& task) {
    os << "timepoint = " << task.runtime << "\n"
       << "interval  = " << task.interval << "\n"
       << "periodic  = " << task.periodic;
    return os;
}


bool operator<(Task const& lhs, Task const& rhs) {
    return lhs.runtime < rhs.runtime;
}


template <typename TaskCollection>
struct TimerTemplate {
    timer_t        timerid;
    std::uint64_t  ticks;
    TaskCollection tasks;
};


namespace first {
    struct Timer : TimerTemplate<std::list<Task>> {
    };


    void AddTask(Timer& timer, Task&& task) {
        timer.tasks.emplace_back(std::move(task));
    }


    void Schedule(Timer& timer) {
        auto it = timer.tasks.begin();
        while (it != timer.tasks.end()) {
            if (timer.ticks == it->runtime) {
                if (it->periodic) {
                    BOOST_SCOPE_EXIT_ALL(&) { it = std::next(it); };
                    it->handler();
                    it->runtime += it->interval;
                } else {
                    BOOST_SCOPE_EXIT_ALL(&) { it = timer.tasks.erase(it); };
                    it->handler();
                }
            } else {
                it = std::next(it);
            }
        }
    }
} // namespace first


namespace second {
    struct Timer : TimerTemplate<std::list<Task>> {
    };

    void AddTask(Timer& timer, Task&& task) {
        auto const pos = std::upper_bound(timer.tasks.cbegin(), timer.tasks.cend(), task,
                                          [](auto const& a, auto const& b) -> bool {
                                              return a.runtime < b.runtime;
                                          });
        timer.tasks.insert(pos, std::move(task));
    }


    void Schedule(Timer& timer) {
        auto it = timer.tasks.begin();
        while (it != timer.tasks.end()) {
            if (timer.ticks == it->runtime) {
                if (it->periodic) {
                    BOOST_SCOPE_EXIT_ALL(&) {
                        Task tmp(std::move(*it));
                        tmp.runtime += tmp.interval;
                        it = timer.tasks.erase(it);
                        AddTask(timer, std::move(tmp));
                    };

                    it->handler();
                } else {
                    BOOST_SCOPE_EXIT_ALL(&) { it = timer.tasks.erase(it); };
                    it->handler();
                }
            } else {
                // Encounter the first non-expired task, we're done.
                break;
            }
        }
    }
} // namespace second


namespace third {
    struct Timer : TimerTemplate<std::set<Task>> {
    };

    void AddTask(Timer& timer, Task&& task) {
        timer.tasks.insert(std::move(task));
    }


    void Schedule(Timer& timer) {
        auto it = timer.tasks.begin();
        while (it != timer.tasks.end()) {
            if (timer.ticks == it->runtime) {
                if (it->periodic) {
                    BOOST_SCOPE_EXIT_ALL(&) {
                        Task tmp(std::move(*it));
                        tmp.runtime += tmp.interval;
                        it = timer.tasks.erase(it);
                        AddTask(timer, std::move(tmp));
                    };

                    it->handler();
                } else {
                    BOOST_SCOPE_EXIT_ALL(&) { it = timer.tasks.erase(it); };
                    it->handler();
                }
            } else {
                break;
            }
        }
    }
} // namespace third


using timer_type = third::Timer;

void Test() {
    std::cout << "1" << std::endl;
}


static void TimerHandler(int, siginfo_t* si, void*) {
    auto timer = reinterpret_cast<timer_type*>(si->si_value.sival_ptr);
    Schedule(*timer);
    ++timer->ticks;
}


int main() {
    /// Setup signal handler.
    struct sigaction sa;
    sa.sa_flags     = SA_SIGINFO;
    sa.sa_sigaction = TimerHandler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMIN, &sa, NULL) == -1)
        return EXIT_FAILURE;

    /// Create the timer.
    timer_type timer;
    AddTask(timer, Task{ timer.ticks, 2000, true, Test });
    sigevent sev;
    sev.sigev_notify          = SIGEV_SIGNAL;
    sev.sigev_signo           = SIGRTMIN;
    sev.sigev_value.sival_ptr = &timer;
    if (timer_create(CLOCK_REALTIME, &sev, &timer.timerid) == -1)
        return EXIT_FAILURE;

    std::cout << "timer id is " << timer.timerid << std::endl;

    /// Start (arm) the timer.
    long long const bookKeepingInterval = 1000000;
    itimerspec its;
    its.it_value.tv_sec     = bookKeepingInterval / 1000000000;
    its.it_value.tv_nsec    = bookKeepingInterval % 1000000000;
    its.it_interval.tv_sec  = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;
    if (timer_settime(timer.timerid, 0, &its, NULL) == -1)
        return EXIT_FAILURE;

    while (true) {
        sleep(1);
    }

    return EXIT_SUCCESS;
}
