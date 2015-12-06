#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


std::vector<std::string> wordsList{
    "a", "abandon", "ability", "able", "about", "above", "abroad", "absence", "absolute", "absolutely", "absorb",
    "abuse", "academic", "accept", "acceptable", "access", "accident", "accommodation", "accompany", "according",
    "account", "accurate", "accuse", "achieve", "achievement", "acid", "acknowledge", "acquire", "across", "act",
    "action", "active", "activist", "activity", "actor", "actual", "actually", "ad", "adapt", "add", "addition",
    "additional", "address", "adequate", "adjust", "administration", "administrative", "admire", "admission", "admit",
    "adopt", "adult", "advance", "advanced", "advantage"
};


class intermediate_formatter {
public:
    intermediate_formatter(std::string const& prefix, std::string const& suffix)
        : m_prefix{prefix}, m_suffix{suffix} {}

    auto operator()(std::string const& origin) const -> std::string {
        return m_prefix + origin + m_suffix;
    }

private:
    std::string const m_prefix;
    std::string const m_suffix;
};


int main() {
    intermediate_formatter const wordFormatter{"\"", "\","};
    intermediate_formatter const lastWordFormatter{"\"", "\""};

    // concatenate sufficient number of words to create a line
    // without exceeding the maximum length
    auto createLine = [](auto first, auto last, auto formatter,
                         std::size_t lineMaxLen, std::string const& separator,
                         std::string& line) {
        if (first == last) return last;

        auto it = first;
        line += formatter(it++);
        for (; it != last; ++it) {
            auto&& formattedWord = formatter(it);
            auto tryLen = line.size() + separator.size() + formattedWord.size();
            if (tryLen > lineMaxLen) {
                break;
            }
            line += separator + formattedWord;
        }

        return it;
    };

    // format word bases on its position
    auto formatter = [&](auto it) {
        if (it + 1 == wordsList.cend()) // last word
            return lastWordFormatter(*it);
        return wordFormatter(*it);
    };

    std::vector<std::string> lines;
    auto it  = wordsList.cbegin();
    auto eit = wordsList.cend();
    while (it != eit) {
        std::string line{"    "};
        it = createLine(it, eit, formatter, 80, " ", line);
        lines.emplace_back(line);
    }

    for (auto const& line : lines) {
        std::cout << line << "\n";
    }
}
