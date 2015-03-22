#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>


class ThreadObject {
public:
    ThreadObject() {
        // to start a member function as a thread, we need to bind it first
        // yes, for the hidden this pointer
        auto f = std::bind(&ThreadObject::logging, this);

        // start new thread now
        t_ = std::move(std::thread(f));
    }


    ~ThreadObject() {
        // we're done
        stop_ = true;

        // wake the thread up if it's still in the waiting state
        logging_cv_.notify_one();

        // wait for the thread to finish
        if (t_.joinable())
            t_.join();
    }


    void add_data(const std::string& s) {
        // lock the mutex before accessing data
        std::lock_guard<std::mutex> lg(data_mutex_);

        // push data to vector
        data_.push_back(s);

        // wake the logging thread up to process the data
        logging_cv_.notify_one();
    }


private:
    void logging() {
        while (!stop_) {
            // waiting for notification
            std::mutex logging_mutex_;
            std::unique_lock<std::mutex> ul(logging_mutex_);
            logging_cv_.wait(ul);

            // process data
            std::lock_guard<std::mutex> lg(data_mutex_);
            if (data_.size() >= 2) {
                for (const auto& s : data_) {
                    std::cout << s << std::endl;
                }
                data_.clear();
            }
        }

        // process left-data before stopping completely
        std::lock_guard<std::mutex> lg(data_mutex_);
        for (const auto& s : data_) {
            std::cout << s << std::endl;
        }
    }


private:
    std::thread t_;
    std::atomic<bool> stop_ = false;
    std::vector<std::string> data_;
    std::mutex data_mutex_;
    std::condition_variable logging_cv_;
};


int main() {
    ThreadObject to;
    to.add_data("1");
    to.add_data("2");
    to.add_data("3");
    to.add_data("4");
    to.add_data("5");
}
