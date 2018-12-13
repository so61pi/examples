#include <iostream>
#include <sstream>
#include <string>
#include <vector>


enum class TokenKind {
    Unknown,

    OpenBracket,
    CloseBracket,
    Equal,
    Identifier,
    NewLine,

    EndOfFile
};


struct Token {
    TokenKind kind;
    char const* begin;
    char const* end;
};


std::string ToReadable(Token const& tok) {
    if (tok.begin == tok.end) {
        if (tok.kind == TokenKind::EndOfFile) {
            return "[EOF]";
        }
        return "[empty]";
    }

    std::string result;
    for (char const* it = tok.begin; it != tok.end; ++it) {
        switch (*it) {
        case '\r':
            result += "\\r";
            break;

        case '\n':
            result += "\\n";
            break;

        case '\t':
            result += "\\t";
            break;

        default:
            result += *it;
        }
    }
    return result;
}


std::ostream& operator<<(std::ostream& os, Token const& tok) {
    char const* tokname = "";
    switch (tok.kind) {
    case TokenKind::Unknown:
        tokname = "Unknown";
        break;

    case TokenKind::OpenBracket:
        tokname = "OpenBracket";
        break;

    case TokenKind::CloseBracket:
        tokname = "CloseBracket";
        break;

    case TokenKind::Equal:
        tokname = "Equal";
        break;

    case TokenKind::Identifier:
        tokname = "Identifier";
        break;

    case TokenKind::NewLine:
        tokname = "NewLine";
        break;

    case TokenKind::EndOfFile:
        tokname = "EndOfFile";
        break;
    }

    os << '<' << tokname << ':' << ToReadable(tok) << '>';
    return os;
}


struct Location {
    unsigned int line;
    unsigned int column;
};


std::ostream& operator<<(std::ostream& os, Location const& loc) {
    os << "(line = " << loc.line << ", column = " << loc.column << ')';
    return os;
}


Location GetLocation(char const* source, Token const& tok) {
    Location loc{ 1, 1 };
    for (; source != tok.begin; ++source) {
        if (*source == '\n') {
            ++loc.line;
            loc.column = 1;
        } else {
            ++loc.column;
        }
    }
    return loc;
}


class Lexer {
public:
    explicit Lexer(char const* source) : mCurrent(source) {}


    std::vector<Token> Lex() {
        std::vector<Token> toks;
        while (*mCurrent) {
            toks.push_back(LexToken());
        }

        // tokenize EndOfFile
        toks.push_back(LexToken());

        return toks;
    }


    bool IsValidCharForIdentifier(char c) {
        return ('0' <= c && c <= '9') ||
               ('A' <= c && c <= 'Z') ||
               ('a' <= c && c <= 'z') ||
               (c == '-' || c == '_');
    }


    bool LexIdentifier(Token& tok) {
        tok.kind = TokenKind::Identifier;
        tok.begin = mCurrent;
        while (IsValidCharForIdentifier(*mCurrent)) {
            ++mCurrent;
        }
        tok.end = mCurrent;
        return true;
    }


    Token LexToken() {
        Token tok;

        switch (*mCurrent) {
        case '\0':
            tok.kind = TokenKind::EndOfFile;
            tok.begin = mCurrent;
            tok.end = mCurrent;
            break;

        case '\n':
            tok.kind = TokenKind::NewLine;
            tok.begin = mCurrent;
            tok.end = mCurrent + 1;
            ++mCurrent;
            break;

        case '[':
            tok.kind = TokenKind::OpenBracket;
            tok.begin = mCurrent;
            tok.end = mCurrent + 1;
            ++mCurrent;
            break;

        case ']':
            tok.kind = TokenKind::CloseBracket;
            tok.begin = mCurrent;
            tok.end = mCurrent + 1;
            ++mCurrent;
            break;

        case '=':
            tok.kind = TokenKind::Equal;
            tok.begin = mCurrent;
            tok.end = mCurrent + 1;
            ++mCurrent;
            break;

        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case '0': case '1': case '2': case '3': case '4': case '5': case '6':
        case '7': case '8': case '9':
        case '_': case '-':
            if (LexIdentifier(tok) == false) {
                tok.kind = TokenKind::Unknown;
                tok.begin = tok.end = nullptr; 
            }
            break;

        default:
            tok.kind = TokenKind::Unknown;
            tok.begin = tok.end = mCurrent;
            ++mCurrent;
            break;
        }

        return tok;
    }


private:
    char const* mCurrent;
};


struct key_value_t {
    std::string key;
    std::string value;
};


struct section_t {
    std::string name;
    std::vector<key_value_t> kvs;
};


using ini_t = std::vector<section_t>;


std::stringstream error;


bool ParseKeyValue(char const* source, std::vector<Token>::iterator& stok, key_value_t& kv) {
    kv.key.clear();
    kv.value.clear();

    std::vector<Token>::iterator itok = stok;

    if (itok->kind != TokenKind::Identifier) {
        error << "Key=Value : expect key-identifier, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    kv.key = std::string(itok->begin, itok->end);
    ++itok;

    if (itok->kind != TokenKind::Equal) {
        error << "Key=Value : expect = after key-identifier, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    ++itok;

    if (itok->kind != TokenKind::Identifier) {
        error << "Key=Value : expect value-identifier, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    kv.value = std::string(itok->begin, itok->end);
    ++itok;

    if (itok->kind != TokenKind::NewLine) {
        error << "Key=Value : expect new-line after key=value, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    ++itok;

    stok = itok;
    return true;
}


bool ParseSection(char const* source, std::vector<Token>::iterator& stok, section_t& sec) {
    sec.name.clear();
    sec.kvs.clear();

    std::vector<Token>::iterator itok = stok;

    if (itok->kind != TokenKind::OpenBracket) {
        error << "Section : expect [ for section, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    ++itok;

    if (itok->kind != TokenKind::Identifier) {
        error << "Section : expect section-name, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    sec.name = std::string(itok->begin, itok->end);
    ++itok;

    if (itok->kind != TokenKind::CloseBracket) {
        error << "Section : expect ] for section, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    ++itok;

    if (itok->kind != TokenKind::NewLine) {
        error << "Section : expect new-line after section-name, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    ++itok;

    key_value_t kv;
    while (ParseKeyValue(source, itok, kv)) {
        sec.kvs.push_back(kv);
    }

    if (itok->kind != TokenKind::NewLine) {
        error << "Section : expect new-line after section, get " << *itok << " at " << GetLocation(source, *itok) << "\n";
        return false;
    }
    ++itok;

    // complete a section, clear all errors
    error.str("");

    stok = itok;
    return true;
}


bool ParseIni(char const* source, std::vector<Token>::iterator& stok, ini_t& ini) {
    section_t sec;
    while (ParseSection(source, stok, sec)) {
        ini.push_back(sec);
    }

    if (stok->kind != TokenKind::EndOfFile) {
        error << "Ini : expect end-of-file, get " << *stok << " at " << GetLocation(source, *stok) << "\n";
        return false;
    }

    return true;
}


int main() {
    char const* source =
        "[abc]\n"
        "aaa=bbb\n"
        "ccc=ccc\n"
        "\n"
        "[def]\n"
        "ddd=eee\n"
        "\n"
        ;

    Lexer lexer(source);
    std::vector<Token> toks = lexer.Lex();

    ini_t ini;
    std::vector<Token>::iterator stok = toks.begin();
    if (ParseIni(source, stok, ini) == false) {
        std::cout << error.str();
    } else {
        std::cout << "Parse successfully!" << std::endl;
        for (auto&& sec : ini) {
            std::cout << sec.name << '\n';
            for (auto&& kv : sec.kvs) {
                std::cout << "  " << kv.key << '=' << kv.value << '\n';
            }
        }
    }
}
