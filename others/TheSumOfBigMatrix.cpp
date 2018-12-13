#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

//
// my CPU
//
// Intel Core i5 2500K
// L1 Data: 128KB
// L1 Instruction: 128KB
// L2: 1MB
// L3: 6MB
//

//
// calculate the sum of all the elements in the matrix
//
int main() {
    std::chrono::seconds sleep_duration(5);
    const size_t MAX_ROWS = 64; // arbitrary number, just make sure you have enough RAM
                                // the bigger this number, the slower when using column by column method
                                // (in comparison with row by row method)
    const size_t MAX_COLS = 6 * 1024 * 1024; // equal to L3 cache size above
    
    std::unique_ptr<unsigned char[]> matrix(new unsigned char[MAX_ROWS * MAX_COLS]);
    for (size_t i = 0; i < MAX_ROWS * MAX_COLS; ++i)
        matrix[i] = i % 256;

    //
    // column by column
    //
    // when you add up number in a matrix this way, it's very slow
    // because it doesn't make use of cache in a good way
    // every time you want to access an element in the matrix,
    // the chance it is in the RAM (instead of in the cache) is bigger than
    // than when you use row by row method
    //
    std::this_thread::sleep_for(sleep_duration);
    long long columnTime = 0;
    {
        auto start = std::chrono::steady_clock::now();
        // add up
        size_t sum = 0;
        for (size_t col = 0; col < MAX_COLS; ++col)
            for (size_t row = 0; row < MAX_ROWS; ++row)
                sum += matrix[row * MAX_COLS + col];
        auto stop = std::chrono::steady_clock::now();
        columnTime = (stop - start).count();
        std::cout << "Column by column (consumed time): " << columnTime << "\n"
            << "The sum: " << sum << "\n\n";
    }
    
    //
    // row by row
    //
    // this version is better, make good use of cache
    // and of course, have the same result (the value of sum variable)
    //
    std::this_thread::sleep_for(sleep_duration);
    long long rowTime = 0;
    {
        auto start = std::chrono::steady_clock::now();
        // add up
        size_t sum = 0;
        for (size_t row = 0; row < MAX_ROWS; ++row)
            for (size_t col = 0; col < MAX_COLS; ++col)
                sum += matrix[row * MAX_COLS + col];
        auto stop = std::chrono::steady_clock::now();
        rowTime = (stop - start).count();
        std::cout << "Row by row (consumed time): " << rowTime << "\n"
            << "The sum: " << sum << "\n\n";
    }
    
    std::cout << "Ratio: " << static_cast<double>(columnTime) / rowTime << std::endl;
    
    //
    // 1 of my results:
    //
    // Column by column (consumed time): 51939716
    // The sum : 4093640704
    //
    // Row by row (consumed time) : 3201801
    // The sum : 4093640704
    //
    // Ratio : 16.222
    //
}
