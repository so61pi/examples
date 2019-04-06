#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


int main() {
    using namespace std;

    vector<int> v;
    cout << setw(20) << "size() before"
         << setw(20) << "capacity() before"
         << setw(20) << "size() after"
         << setw(20) << "capacity() after"
         << setw(20) << "reallocation"
         << setw(20) << "capacity delta\n";
    for (auto i = 0; i < 1000; ++i) {
        auto bs = v.size();
        auto bc = v.capacity();
        v.push_back(i);
        auto as = v.size();
        auto ac = v.capacity();
        auto reallocation = (ac != bc);
        
        cout << setw(20) << bs
             << setw(20) << bc
             << setw(20) << as
             << setw(20) << ac
             << setw(20) << (reallocation ? "<=========" : "")
             << setw(20) << (reallocation ? to_string(ac - bc) : "")
             << "\n";
    }
}