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


int main() {
    for (auto& word : wordsList) {
        word = '"' + word + '"';
    }

    // add comma after each quoted word, except for the last word
    for (auto i = 0U; i + 1 < wordsList.size(); ++i) {
        wordsList[i].push_back(',');
    }

    // concatenate sufficient number of words to create a line
    // without exceeding the maximum length
    auto createLine = [](auto first, auto last, std::size_t lineMaxLen,
                         std::string const& separator, std::string& line) {
        if (first == last) return last;

        auto it = first;
        line += *it++;
        for (; it != last; ++it) {
            auto tryLen = line.size() + separator.size() + it->size();
            if (tryLen > lineMaxLen) {
                break;
            }
            line += separator + *it;
        }

        return it;
    };


    std::vector<std::string> lines;
    auto it  = wordsList.begin();
    auto eit = wordsList.end();
    while (it != eit) {
        std::string line{"    "};
        it = createLine(it, eit, 80, " ", line);
        lines.emplace_back(line);
    }

    for (auto const& line : lines) {
        std::cout << line << "\n";
    }
}
