#include <chrono>
#include <iostream>
#include <memory>
#include <thread>


//
// calculate the sum of all the elements in the matrix
//
int main() {
    using namespace std::literals;

    size_t const ROWS = 64;
    size_t const COLS = 6 * 1024 * 1024;

    std::unique_ptr<unsigned char[]> matrix(new unsigned char[ROWS * COLS]);
    for (size_t i = 0; i < ROWS * COLS; ++i)
        matrix[i] = i % 256;

    // column by column
    std::this_thread::sleep_for(1s);
    long long columnTime = 0;
    {
        auto start = std::chrono::steady_clock::now();
        // add up
        size_t sum = 0;
        for (size_t c = 0; c < COLS; ++c)
            for (size_t r = 0; r < ROWS; ++r)
                sum += matrix[r * COLS + c];
        auto stop  = std::chrono::steady_clock::now();
        columnTime = (stop - start).count();
        std::cout << "Column by column (consumed time): " << columnTime << "\n"
                  << "The sum: " << sum << "\n\n";
    }

    // row by row
    std::this_thread::sleep_for(1s);
    long long rowTime = 0;
    {
        auto start = std::chrono::steady_clock::now();
        // add up
        size_t sum = 0;
        for (size_t r = 0; r < ROWS; ++r)
            for (size_t c = 0; c < COLS; ++c)
                sum += matrix[r * COLS + c];
        auto stop = std::chrono::steady_clock::now();
        rowTime   = (stop - start).count();
        std::cout << "Row by row (consumed time): " << rowTime << "\n"
                  << "The sum: " << sum << "\n\n";
    }

    std::cout << "Ratio: " << static_cast<double>(columnTime) / rowTime
              << std::endl;

}
