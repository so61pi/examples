#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>


template <typename Clock>
class Period {
public:
    auto get() const { return Clock::now() - m_last; }
    void reset() { m_last = Clock::now(); }
    
private:
    typename Clock::time_point m_last = Clock::now();
};



class MemAllocatorBase {
public:
    using pointer_t = std::shared_ptr<void>;


    template <typename T, typename... Args>
    auto allocate(Args&&... args) {
        std::shared_ptr<T> ptr;
        m_ptrs.emplace_back(sizeof(T), nullptr);
        try {
            ptr = std::make_shared<T>(std::forward<Args>(args)...);
        } catch (...) {
            m_ptrs.pop_back();
            throw;
        }

        m_ptrs.back().second = ptr;
        m_total += m_ptrs.back().first;
        
        if (m_total > 5*1024*1024) sweep();
        return ptr;
    }


    void sweep() noexcept {
        m_ptrs.erase(std::remove_if(m_ptrs.begin(), m_ptrs.end(),
                                    [this](std::pair<std::size_t, pointer_t> const& ptr) {
                                        if (ptr.second.unique()) {
                                            m_total -= ptr.first;
                                            return true;
                                        }
                                        return false;
                                    }),
                     m_ptrs.end());
    }


    auto size() const noexcept -> std::size_t { return m_total; }
    auto count() const noexcept -> std::size_t { return m_ptrs.size(); }


private:
    std::vector<std::pair<std::size_t, pointer_t>> m_ptrs;
    std::size_t m_total = 0;
};


class MemAllocator : public MemAllocatorBase {
    using mutex_t = std::mutex;
    using base = MemAllocatorBase;


public:
    MemAllocator() {
        m_thread = std::thread{[this]{ period_run(); }};
    }


    MemAllocator(MemAllocator const&) = delete;
    MemAllocator(MemAllocator&&) = default;
    void operator=(MemAllocator const&) = delete;
    void operator=(MemAllocator&&) = delete;


    template <typename T, typename... Args>
    auto allocate(Args&&... args) {
        std::lock_guard<mutex_t> lg{m_mutex};
        return base::allocate<T>(std::forward<Args>(args)...);
    }


    void sweep() {
        std::lock_guard<mutex_t> lg{m_mutex};
        base::sweep();
    }


    ~MemAllocator() {
        { // exit period run thread
            std::lock_guard<mutex_t> lg{m_mutex};
            m_exit = true;
            m_cv.notify_one();
        }
        m_thread.join();
    }


private:
    void period_run() {
        std::unique_lock<mutex_t> lk{m_mutex};
        while (true) {
            m_cv.wait_for(lk, std::chrono::milliseconds{10});
            if (m_exit) return;
            base::sweep();
        }
    }


private:
    mutex_t m_mutex;
    std::condition_variable m_cv;
    bool m_exit = false;
    std::thread m_thread;
};

 
int main() {
    MemAllocator ma;
    for (auto i = 0; i < 50*1000; ++i) {
        auto a = ma.allocate<int>(i);
        auto b = ma.allocate<long long>(i+1);
        auto c = ma.allocate<std::string>("a very very very very very very long sentence");
        std::cout << std::setw(10) << i
                  << std::setw(20) << ma.size()
                  << std::setw(20) << ma.count()
                  << "\n";
    }
}
