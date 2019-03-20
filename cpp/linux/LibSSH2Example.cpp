#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <boost/scope_exit.hpp>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <libssh2.h>


using namespace std::literals;


#define AuthTypeUnknown 0
#define AuthTypePassword (1 << 0)
#define AuthTypeKeyboardInteractive (1 << 1)
#define AuthTypePublicKey (1 << 2)


char const* const gUsername = "vagrant";
char const* const gPassword = "vagrant";
char const* const gPublicKeyFile = "~/.ssh/key.pub";
char const* const gPrivateKeyFile = "~/.ssh/key";
char const* const gServerIp = "127.0.0.1";
int const         gServerPort = 2222;


void ShowSessionError(LIBSSH2_SESSION* session, char const* const message) {
    char* errmsg = nullptr;
    int msglen = 0;
    ::libssh2_session_last_error(session, &errmsg, &msglen, 0);
    std::cerr << "[error] " << message << '\n'
              << "        " << errmsg << std::endl;
}


void ShowError(int errnum) {
    std::cerr << "[error] " << std::strerror(errnum) << std::endl;
}


void ShowError(std::string const& message) {
    std::cerr << "[error] " << message << std::endl;
}


void ShowInfo(std::string const& message) {
    std::cout << "[info] " << message << std::endl;
}


void KbdCallback(char const* name, int nameLen,
                  char const* instruction, int instructionLen,
                  int numPrompts, LIBSSH2_USERAUTH_KBDINT_PROMPT const* prompts, LIBSSH2_USERAUTH_KBDINT_RESPONSE* responses,
                  void** abstract)
{
    (void) abstract;

    ShowInfo("keyboard-interactive authentication.");
    ShowInfo("Authenticate name : " + std::string(name, name + nameLen));
    ShowInfo("Instruction : " + std::string(instruction, instruction + instructionLen));
    for (auto i = 0; i < numPrompts; ++i) {
        ShowInfo("Prompt : " + std::string(prompts[i].text, prompts[i].text + prompts[i].length));

        std::string res;
        if (std::getline(std::cin, res)) {
            responses[i].text = strdup(res.c_str());
            responses[i].length = res.size();
        }
    }
}


bool ChannelReadAndDisplay(LIBSSH2_SESSION* session, LIBSSH2_CHANNEL* channel) {
    while (true) {
        char buffer[100] = {};
        auto bytes = ::libssh2_channel_read(channel, buffer, sizeof(buffer));
        if (bytes > 0) {
            std::cout << std::string(buffer, buffer + bytes);
        } else if (bytes == 0) {
            break;
        } else {
            if (bytes == LIBSSH2_ERROR_EAGAIN) {
                break;
            } else {
                ShowSessionError(session, "Cannot read from channel");
                return false;
            }
        }
    }
    return true;
}


bool ReadCommandAndExec(LIBSSH2_SESSION* session, LIBSSH2_CHANNEL* channel, bool& end) {
    std::string cmd;
    if (!std::getline(std::cin, cmd)) {
        ShowError("Cannot get command from user.");
        return false;
    }

    if (::libssh2_channel_eof(channel) && cmd == "exit") {
        end = true;
        return true;
    }

    cmd += "\r\n";
    if (::libssh2_channel_write(channel, cmd.c_str(), cmd.size()) < 0) {
        ShowSessionError(session, "Failed to write command to server.");
        return false;
    }

    return true;
}


bool UserAuthAgent(LIBSSH2_SESSION* session) {
    // connect to ssh-agent
    auto const agent = libssh2_agent_init(session);
    if (!agent) {
        ShowError("Failure initializing ssh-agent support.");
        return false;
    }
    BOOST_SCOPE_EXIT_ALL(&) { libssh2_agent_free(agent); };

    if (libssh2_agent_connect(agent)) {
        ShowError("Failure connecting to ssh-agent");
        return false;
    }
    BOOST_SCOPE_EXIT_ALL(&) { libssh2_agent_disconnect(agent); };

    if (libssh2_agent_list_identities(agent)) {
        ShowError("Failure requesting identities to ssh-agent");
        return false;
    }

    libssh2_agent_publickey* identity = nullptr;
    libssh2_agent_publickey* prevIdentity = nullptr;
    while (true) {
        auto const rc = libssh2_agent_get_identity(agent, &identity, prevIdentity);
        if (rc == 1) {
            ShowError("Couldn't continue authentication");
            return false;
        }

        if (rc < 0) {
            ShowError("Failure obtaining identity from ssh-agent support");
            return false;
        }

        if (libssh2_agent_userauth(agent, gUsername, identity)) {
            ShowError("Authentication with username "s + gUsername + " and public key " + identity->comment + " failed!");
        } else {
            ShowInfo("Authentication with username "s + gUsername + " and public key " + identity->comment + " succeeded!");
            break;
        }
        prevIdentity = identity;
    }

    return true;
}


bool UserAuth(LIBSSH2_SESSION* session) {
    auto const authlist = ::libssh2_userauth_list(session, gUsername, std::strlen(gUsername));
    if (!authlist) {
        ShowSessionError(session, "Failed to get auth list.");
        return false;
    }

    int authtype = AuthTypeUnknown;
    if (std::strstr(authlist, "publickey")) {
        ShowInfo("Can use publickey authentication method.");
        authtype |= AuthTypePublicKey;
    }
    if (std::strstr(authlist, "password")) {
        ShowInfo("Can use password authentication method.");
        authtype |= AuthTypePassword;
    }
    if (std::strstr(authlist, "keyboard-interactive")) {
        ShowInfo("Can use keyboard-interactive authentication method.");
        authtype |= AuthTypeKeyboardInteractive;
    }

    if (authtype & AuthTypePublicKey) {
        if (UserAuthAgent(session)) {
            ShowInfo("Authenticate successfully by using publickey with ssh-agent.");
            return true;
        }

        if (::libssh2_userauth_publickey_fromfile(session, gUsername, gPublicKeyFile, gPrivateKeyFile, gPassword) == 0) {
            ShowInfo("Authenticate successfully by using publickey.");
            return true;
        }

        ShowSessionError(session, "Failed to authenticate by public key.");
    }

    if (authtype & AuthTypePassword) {
        if (::libssh2_userauth_password(session, gUsername, gPassword) == 0) {
            ShowInfo("Authenticate successfully by using password.");
            return true;
        }
        ShowSessionError(session, "Failed to authenticate by password.");
    }

    if (authtype & AuthTypeKeyboardInteractive) {
        if (::libssh2_userauth_keyboard_interactive(session, gUsername, KbdCallback) == 0) {
            ShowInfo("Authenticate successfully by using keyboard-interactive.");
            return true;
        }
        ShowSessionError(session, "Failed to authenticate by keyboard.");
    }

    ShowError("No authenticating method works.");
    return false;
}


int main() {
    if (::libssh2_init(0) != 0) {
        ShowError("Cannot initialize libssh2");
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { ::libssh2_exit(); };

    auto const libssh2sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (libssh2sock < 0) {
        ShowError(errno);
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { ::close(libssh2sock); };

    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(gServerPort);
    sin.sin_addr.s_addr = inet_addr(gServerIp);
    if (::connect(libssh2sock, reinterpret_cast<sockaddr*>(&sin), sizeof(sin))) {
        ShowError(errno);
        return EXIT_FAILURE;
    }

    auto const session = ::libssh2_session_init();
    BOOST_SCOPE_EXIT_ALL(&) {
        ::libssh2_session_disconnect(session, "normal shutdown");
        ::libssh2_session_free(session);
    };

    if (::libssh2_session_handshake(session, libssh2sock) < 0) {
        ShowSessionError(session, "Cannot establish SSH connection.");
        return EXIT_FAILURE;
    }

    auto const sha1HashLen = 20;
    auto const md5HashLen = 16;
    auto fingerprint = ::libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
    if (!fingerprint) {
        ShowSessionError(session, "Failed to get hostkey hash.");
        return EXIT_FAILURE;
    }
    for (auto i = 0; i < sha1HashLen; ++i) {
        std::cout << std::hex << unsigned(fingerprint[i]) << ' ';
    }
    std::cout << std::endl;

    if (!UserAuth(session)) {
        return EXIT_FAILURE;
    }

    // open a generic channel
    auto const channel = ::libssh2_channel_open_session(session);
    if (!channel) {
        ShowSessionError(session, "Failed to open channel.");
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { ::libssh2_channel_free(channel); };
    ShowInfo("Open channel successfully.");

    // request a pty
    if (::libssh2_channel_request_pty(channel, "vanilla")) {
        ShowSessionError(session, "Failed to request a pty.");
        return EXIT_FAILURE;
    }
    ShowInfo("Request a pty successfully.");

    // change pty size
    auto const ptyWidth  = 120;
    auto const ptyHeight = 80;
    if (::libssh2_channel_request_pty_size(channel, ptyWidth, ptyHeight)) {
        ShowSessionError(session, "Failed to change pty size.");
        return EXIT_FAILURE;
    }
    ShowInfo("Change pty size successfully.");

    // open a shell on that pty
    if (::libssh2_channel_shell(channel)) {
        ShowSessionError(session, "Failed to open a shell.");
        return EXIT_FAILURE;
    }
    ShowInfo("Open a shell successfully.");

    // libssh2 socket is always non-blocking
    // this is set up in libssh2_session_handshake
    //
    // libssh2_session_handshake
    // `-session_startup
    //   `-session_nonblock
    //     `-fcntl(O_NONBLOCK)
    //
    // libssh2_channel/session_set_blocking only changes api_block_mode
    //
    // +------------------+----------------------------------+
    // |  api_block_mode  | libssh2 api behaviour for EAGAIN |
    // +------------------+----------------------------------+
    // | 0 (non-blocking) |    return LIBSSH2_ERROR_EAGAIN   |
    // +------------------+----------------------------------+
    // |   1 (blocking)   | loop (BLOCK_ADJUST in session.h) |
    // +------------------+----------------------------------+
    ::libssh2_channel_set_blocking(channel, 0);

    constexpr int MAXCONN = 2;
    auto const epfd = epoll_create(MAXCONN);
    if (epfd < 0) {
        ShowError(errno);
        return EXIT_FAILURE;
    }

    epoll_event ev = {};
    ev.events = EPOLLIN;
    ev.data.fd = STDIN_FILENO;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev) < 0) {
        ShowError(errno);
        return EXIT_FAILURE;
    }

    ev.data.fd = libssh2sock;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, libssh2sock, &ev) < 0) {
        ShowError(errno);
        return EXIT_FAILURE;
    }

    while (true) {
        epoll_event events[MAXCONN] = {};
        auto ready = epoll_wait(epfd, events, MAXCONN, -1);
        if (ready < 0) {
            if (errno == EINTR) continue;
            ShowError(errno);
            return EXIT_FAILURE;
        }

        for (auto i = 0; i < ready; ++i) {
            auto const es = events[i].events;
            auto const fd = events[i].data.fd;

            if (es & EPOLLIN) {
                if (fd == libssh2sock) {
                    if (ChannelReadAndDisplay(session, channel) == false) {
                        return EXIT_FAILURE;
                    }
                } else { // fd == STDIN_FILENO
                    bool end = false;
                    if (ReadCommandAndExec(session, channel, end) == false) {
                        return EXIT_FAILURE;
                    }
                    if (end) return EXIT_SUCCESS;
                }
            } else if (es & (EPOLLHUP | EPOLLERR)) {
                ShowError("Connection closed.");
                return EXIT_FAILURE;
            }
        }
    }
}
