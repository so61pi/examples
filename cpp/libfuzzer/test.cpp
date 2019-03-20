#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


using namespace std;


struct InputData {
    unsigned Bits;
    float A;
    float B;
    float C;
};

void TestMe(InputData const&);


extern "C" int LLVMFuzzerTestOneInput(uint8_t const* data, size_t size) {
    if (size != sizeof(InputData)) {
        return 0;
    }

    TestMe(*reinterpret_cast<InputData const*>(data));
    return 0;
}


void TestMe(InputData const& data) {
    bool const activated = (data.Bits & (1 << 1)) && (data.Bits & (1 << 15)) && (data.Bits & (1 << 30));
    if (!activated) {
        return;
    }


    double A = data.A;
    double B = data.B;
    double C = data.C;


    for (int i = 0; i < 10; ++i) {
        if (A == 0) {
            A = 1;
        }

        if (i == 9) {
            if (14 < A && A < 16) {
                A = A*2;
                if (49 < B && B < 51) {
                    B -= 1;
                }
            }
        }

        double const delta = B * B - 4 * A * C;
        if (delta > 0) {
            double const x1 = (-B + sqrt(delta))/(2*A);
            double const x2 = (-B - sqrt(delta))/(2*A);
            A += x1;
            B += x2;
            if (x1 < x2) {
                B -= x2;
            }
        } else if (delta == 0) {
            double const x = -B/(2*A);
            A += x;
        } else {
            B += 1;
        }

        if (A >= (B + C)) {
            B = -B;
        } else {
            A = -A;
        }
    }


    bool const impossible = (A+B>C && B+C>A && C+A>B && B-C>A);
    if (impossible) {
        std::cout << "Impossible\n"
                  << A << "\n"
                  << B << "\n"
                  << C << "\n";
    }

    return;
}


// int main() {
// }
