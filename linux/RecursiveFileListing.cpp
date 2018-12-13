#include <iostream>
#include <boost/scope_exit.hpp>
#include <fts.h>


int main() {
    char const* argv[] = { ".", nullptr };
    auto const ftsp = fts_open(const_cast<char* const*>(argv), FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, nullptr);
    if (!ftsp) return 1;
    BOOST_SCOPE_EXIT_ALL(&) { fts_close(ftsp); };

    auto const chp = fts_children(ftsp, 0);
    if (!chp) return 1;

    FTSENT* p;
    while ((p = fts_read(ftsp))) {
        if (p->fts_info == FTS_F) {
            std::cout << "fts_name = " << p->fts_name << "\n"
                      << "fts_path = " << p->fts_path << "\n"
                      << std::endl;
        }
    }

    return 0;
}
