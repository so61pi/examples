#include <iostream>
#include <tuple>


class Color {
public:
    Color(int red, int green, int blue, int alpha) :
        m_red{ red },
        m_green{ green },
        m_blue{ blue },
        m_alpha{ alpha}
    {}

    friend bool operator==(Color const& lhs, Color const& rhs);

private:
    int m_red;
    int m_green;
    int m_blue;
    int m_alpha;
};


// using std::tie to do comparison
bool operator==(Color const& lhs, Color const& rhs) {
    return std::tie(lhs.m_red, lhs.m_green, lhs.m_blue, lhs.m_alpha)
        == std::tie(rhs.m_red, rhs.m_green, rhs.m_blue, rhs.m_alpha);
}


int main() {
    Color green{ 0, 0xFF, 0, 0xFF };
    Color blue{ 0, 0, 0xFF, 0xFF };

    std::cout << (green == blue) << "\n"
        << (green == green) << "\n";
}
