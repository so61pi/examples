#include <string>
#include <utility>
#include <vector>

#include <iostream>

#include "spl.hpp"


using namespace spl::operators;


namespace ini {
    struct key_value_t {
        void clear() {
            key.clear();
            value.clear();
        }

        std::string key;
        std::string value;
    };

    struct section_t {
        void clear() {
            name.clear();
            kvs.clear();
        }

        std::string name;
        std::vector<key_value_t> kvs;
    };

    using ini_t = std::vector<section_t>;

    namespace parser {
        class string_parser : public spl::base_parser {
        public:
            explicit string_parser(std::function<void(std::string const&)> fn) :
                m_fn(fn) {}

            virtual bool parse(spl::iterator& first, spl::iterator const& last,
                               spl::error& error) {
                std::string string;

                auto it = first;
                for (; it != last; ++it) {
                    bool valid = ('a' <= *it && *it <= 'z') ||
                                 ('A' <= *it && *it <= 'Z') ||
                                 ('0' <= *it && *it <= '9') ||
                                 (*it == '_');
                    if (!valid) break;
                    string += *it;
                }

                if (string.empty()) {
                    error.message << "failed at string-parser, empty string\n";
                    error.where = it;
                    return false;
                }
                first = it;

                m_fn(string);
                return true;
            }

        private:
            std::function<void(std::string const&)> m_fn;
        };


        class kv_parser : public spl::base_parser {
        public:
            explicit kv_parser(std::function<void(key_value_t const&)> fn) :
                m_fn(fn) {}

            virtual bool parse(spl::iterator& first, spl::iterator const& last,
                               spl::error& error) {
                key_value_t kv;
                auto save_key = [&](std::string s) { kv.key = s; };
                auto save_val = [&](std::string s) { kv.value = s; };

                auto parser = string_parser(save_key) >> '=' >> string_parser(save_val) >> '\n';
                if (parser.parse(first, last, error) == false) {
                    error.message << "failed at kv-parser\n";
                    return false;
                }

                m_fn(kv);
                return true;
            }

        private:
            std::function<void(key_value_t const&)> m_fn;
        };


        class section_parser : public spl::base_parser {
        public:
            explicit section_parser(std::function<void(section_t const&)> fn) :
                m_fn(fn) {}

            virtual bool parse(spl::iterator& first, spl::iterator const& last,
                               spl::error& error) {
                section_t sec;
                auto save_name = [&](std::string n) { sec.name = n; };
                auto save_kv = [&](key_value_t kv) { sec.kvs.push_back(kv); };

                auto parser =   '[' >> string_parser(save_name) >> "]\n"
                             >> *kv_parser(save_kv)
                             >> '\n'
                             ;

                if (parser.parse(first, last, error) == false) {
                    error.message << "failed at section-parser, invalid section\n";
                    return false;
                }

                
                return true;
            }

        private:
            std::function<void(section_t const&)> m_fn;
        };


        class ini_parser : public spl::base_parser {
        public:
            explicit ini_parser(ini_t& ini) :
                m_ini(ini) {}

            virtual bool parse(spl::iterator& first, spl::iterator const& last,
                               spl::error& error) {
                auto save_sec = [this](section_t sec) { m_ini.push_back(sec); };
                auto parser =   *section_parser(save_sec)
                             >> spl::end_parser()
                             ;
                if (parser.parse(first, last, error) == false) return false;
                return true;
            }

        private:
            ini_t& m_ini;
        };
    }
}


int main() {
    std::string text =
        "[abc]\n"
        "aaa=bbb\n"
        "bbb=ccc\n"
        "\n"
        "[def]\n"
        "ddd=eee\n"
        "\n"
        //"ghi"
    ;

    spl::error error;

    ini::ini_t ini;
    ini::parser::ini_parser parser(ini);
    auto first = spl::iterator(text.cbegin());
    auto last = spl::iterator(text.cend());
    if (parser.parse(first, last, error) == true) {
        std::cout << "success\n"
                  << "number of sections : " << ini.size() << "\n";
        for (auto&& sec : ini) {
            std::cout << "section = " << sec.name << "\n";
            for (auto&& kv : sec.kvs) {
                std::cout << "    " << kv.key << " = " << kv.value << "\n";
            }
            std::cout << "\n";
        }
    } else {
        std::cout << "error\n" << error.message.str() << '\n'
                  << "line   " << error.where.line() << '\n'
                  << "column " << error.where.column() << '\n'
                  << "char   " << *error.where << '\n';
    }
}

