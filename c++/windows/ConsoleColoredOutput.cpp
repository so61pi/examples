#include <iostream>
#include <type_traits>
#include <vector>

#include <windows.h>


enum class color {
    black = 0,
    red   = 1,
    green = 2,
    blue  = 4,

    white   = red   | green | blue,
    yellow  = red   | green,
    cyan    = green | blue,
    magenta = red   | blue,
};

using color_int_type = std::underlying_type_t<color>;


struct colorinfo {
    color color;
    bool intensity;
};

struct bkgcolor : public colorinfo {};
struct frgcolor : public colorinfo {};


auto setfrg(color c, bool i) -> frgcolor {
    frgcolor fc;
    fc.color = c;
    fc.intensity = i;
    return fc;
}


auto setbkg(color c, bool i) -> bkgcolor {
    bkgcolor bc;
    bc.color = c;
    bc.intensity = i;
    return bc;
}


auto colors() {
    auto hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    ::GetConsoleScreenBufferInfo(hStdout, &csbi);
    return csbi.wAttributes;
}


void colors(WORD attrs) {
    auto hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
    ::SetConsoleTextAttribute(hStdout, attrs);
}


auto frg(color c) { return setfrg(c, false); }
auto frgi(color c) { return setfrg(c, true); }
auto bkg(color c) { return setbkg(c, false); }
auto bkgi(color c) { return setbkg(c, true); }


std::ostream& operator<<(std::ostream& os, frgcolor const& c) {
    auto hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (::GetConsoleScreenBufferInfo(hStdout, &csbi) == FALSE)
        return os;
    auto attrs = csbi.wAttributes;
    attrs &= (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY);
    if (c.intensity)
        attrs |= FOREGROUND_INTENSITY;

    auto ci = static_cast<color_int_type>(c.color);
    attrs |= ci & static_cast<color_int_type>(color::blue) ? FOREGROUND_BLUE : 0;
    attrs |= ci & static_cast<color_int_type>(color::green) ? FOREGROUND_GREEN : 0;
    attrs |= ci & static_cast<color_int_type>(color::red) ? FOREGROUND_RED : 0;

    ::SetConsoleTextAttribute(hStdout, attrs);
    return os;
}


std::ostream& operator<<(std::ostream& os, bkgcolor const& c) {
    auto hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    if (::GetConsoleScreenBufferInfo(hStdout, &csbi) == FALSE)
        return os;
    auto attrs = csbi.wAttributes;
    attrs &= (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
    if (c.intensity)
        attrs |= BACKGROUND_INTENSITY;

    auto ci = static_cast<color_int_type>(c.color);
    attrs |= ci & static_cast<color_int_type>(color::blue) ? BACKGROUND_BLUE : 0;
    attrs |= ci & static_cast<color_int_type>(color::green) ? BACKGROUND_GREEN : 0;
    attrs |= ci & static_cast<color_int_type>(color::red) ? BACKGROUND_RED : 0;

    ::SetConsoleTextAttribute(hStdout, attrs);
    return os;
}


int main() {
    auto oldColors = colors();
    std::cout << frgi(color::red) << "hello"
              << bkgi(color::cyan) << " " << bkg(color::black)
              << frg(color::magenta) << "world\n";
    colors(oldColors);
    std::cout << "hello world\n";
}
