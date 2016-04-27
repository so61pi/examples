#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// to convert fd to c++ stream
#include <ext/stdio_filebuf.h>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/vfs.h>
#include <termios.h>
#include <unistd.h>


/*
   Parent                          PTY                         Child
                    +---------------+---------------+
   stdout           |               |               |          stdin
----------->    --->|-------------->|-------------->|--->   ----------->
                    |    Input      |    Output     |
                    |               |               |
                    |               |               |
                    |    MASTER     |    SLAVE      |
                    |               |               |
                    |               |               |
   stdin            |    Output     |    Input      |          stdout
<-----------    <---|<--------------|<--------------|<---   <-----------
                    |               |               |
                    +---------------+---------------+
*/


class fdraii {
public:
    fdraii(int fd)
        : m_fd{ fd } {}

    fdraii(fdraii const&) = delete;
    fdraii(fdraii&&) = delete;
    fdraii& operator=(fdraii const&) = delete;
    fdraii& operator=(fdraii&&) = delete;

    operator int() const noexcept { return get(); }
    int get() const noexcept { return m_fd; }
    void reset() noexcept {
        close(m_fd);
        m_fd = -1;
    }
    int release() noexcept {
        auto retfd = m_fd;
        m_fd = -1;
        return retfd;
    }

    ~fdraii() noexcept {
        if (m_fd >= 0) close(m_fd);
    }

private:
    int m_fd;
};


auto get_system_path_max() {
    auto const path_max = pathconf("/", _PC_PATH_MAX);
    if (path_max < 0) {
        throw std::runtime_error{ "cannot determine maximum path length" };
    }
    return path_max;
}


int main() {
    try {
        auto const path_max = get_system_path_max();

        // create pseudoterminal device"
        fdraii mfd{ posix_openpt(O_RDWR) };
        if (mfd < 0) {
            throw std::runtime_error{ "cannot open master pseudoterminal device" };
        }

        if (grantpt(mfd) < 0) {
            throw std::runtime_error{ "cannot change permission for slave pseudoterminal device" };
        }

        if (unlockpt(mfd) < 0) {
            throw std::runtime_error{ "cannot unlock slave pseudoterminal device" };
        }

        std::vector<char> slave_name;
        slave_name.resize(path_max);
        if (ptsname_r(mfd, slave_name.data(), slave_name.size()) != 0) {
            throw std::runtime_error{ "cannot get slave pseudoterminal device's path" };
        }

        fdraii sfd{ open(slave_name.data(), O_RDWR) };
        if (sfd < 0) {
            throw std::runtime_error{ "cannot open slave pseudoterminal device" };
        }

        // create child process
        auto childpid = fork();
        if (childpid < 0) {
            throw std::runtime_error{ "cannot fork" };
        } else if (childpid == 0) {
            //
            // child
            //

            // close unnecessary inherited fd
            mfd.reset();


            // change terminal settings
            // I don't understand this piece of code!
            termios orig_term_settings;
            if (tcgetattr(sfd, &orig_term_settings) < 0) {
                throw std::runtime_error{ "cannot get terminal settings" };
            }

            termios new_term_settings = orig_term_settings;
            cfmakeraw(&new_term_settings);
            if (tcsetattr(sfd, TCSANOW, &new_term_settings) < 0) {
                throw std::runtime_error{ "cannot change terminal settings" };
            }


            // slave side of the PTY becomes the standard input and outputs of the child process
            if (dup2(sfd, STDIN_FILENO) < 0) {
                throw std::runtime_error{ "cannot dup2 STDIN_FILENO" };
            }

            if (dup2(sfd, STDOUT_FILENO) < 0) {
                throw std::runtime_error{ "cannot dup2 STDOUT_FILENO" };
            }

            if (dup2(sfd, STDERR_FILENO) < 0) {
                throw std::runtime_error{ "cannot dup2 STDERR_FILENO" };
            }

            // all standard input and outputs are set up, we don't need sfd anymore
            sfd.reset();


            // read input from stdin (comes from parent's stdout)
            // then write to stdout (goes to parent's stdin)
            while (true) {
                std::string line;
                if (!std::getline(std::cin, line)) {
                    throw std::runtime_error{ "cannot get line" };
                }

                // must use std::endl to add '\n' and flush
                std::cout << "Child received: " << line << std::endl;
            }
        } else {
            //
            // parent
            //

            // close unnecessary inherited fd
            sfd.reset();


            fdraii mofd{ dup(mfd) };
            if (mofd < 0) {
                throw std::runtime_error{ "cannot dup mfd" };
            }


            // sfd is automatically closed by __gnu_cxx::stdio_filebuf<char> destructor
            __gnu_cxx::stdio_filebuf<char> mfd_in_file_buf{ mfd, std::ios_base::in | std::ios_base::binary };
            mfd.release();
            std::istream mis{ &mfd_in_file_buf };

            __gnu_cxx::stdio_filebuf<char> mfd_out_file_buf{ mofd, std::ios_base::out | std::ios_base::binary };
            mofd.release();
            std::ostream mos{ &mfd_out_file_buf };

            // read input from user and send to child process' stdin
            while (true) {
                std::string userinput;
                std::cout << "Input: ";
                if (!std::getline(std::cin, userinput)) {
                    throw std::runtime_error{ "cannot getline" };
                }

                if (userinput == "quit") {
                    break;
                }

                // write to child process' stdin
                // must use std::endl to add '\n' and flush
                mos << userinput << std::endl;

                // read from child process' stdout
                std::string cstdout;
                if (!std::getline(mis, cstdout)) {
                    throw std::runtime_error{ "cannot child getline" };
                }
                std::cout << cstdout << '\n';
            }
        }

    } catch (std::exception const& e) {
        std::cout << "Exception: " << e.what() << "\n";
        return -1;
    } catch (...) {
        std::cout << "Exception: Unknown exception\n";
    }
}
