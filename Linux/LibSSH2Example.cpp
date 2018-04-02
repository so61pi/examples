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


#define AuthTypeUnknown 0
#define AuthTypePassword 1
#define AuthTypeKeyboardInteractive 2
#define AuthTypePublicKey 4


int SetNonBlocking(int sfd) {
    int flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }

    flags |= O_NONBLOCK;
    if (fcntl(sfd, F_SETFL, flags) == -1) {
        return -1;
    }

    return 0;
}


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


char const* const gUsername = "vagrant";
char const* const gPassword = "vagrant";
char const* const gPublicKeyFile = "~/.ssh/key.pub";
char const* const gPrivateKeyFile = "~/.ssh/key";
char const* const gServerIp = "127.0.0.1";
int const         gServerPort = 2222;


int main() {
    if (::libssh2_init(0) != 0) {
        ShowError("Cannot initialize libssh2");
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { ::libssh2_exit(); };

    auto sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ShowError(errno);
        return EXIT_FAILURE;
    }
    BOOST_SCOPE_EXIT_ALL(&) { ::close(sock); };

    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(gServerPort);
    sin.sin_addr.s_addr = inet_addr(gServerIp);
    if (::connect(sock, reinterpret_cast<sockaddr*>(&sin), sizeof(sin))) {
        ShowError(errno);
        return EXIT_FAILURE;
    }

    auto session = ::libssh2_session_init();
    BOOST_SCOPE_EXIT_ALL(&) {
        ::libssh2_session_disconnect(session, "normal shutdown");
        ::libssh2_session_free(session);
    };

    if (::libssh2_session_handshake(session, sock) < 0) {
        ShowSessionError(session, "Cannot establish SSH connection.");
        return EXIT_FAILURE;
    }

    int const sha1HashLen = 20;
    int const md5HashLen = 16;
    auto fingerprint = ::libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
    if (!fingerprint) {
        ShowSessionError(session, "Failed to get hostkey hash.");
        return EXIT_FAILURE;
    }
    for (auto i = 0; i < sha1HashLen; ++i) {
        std::cout << std::hex << unsigned(fingerprint[i]) << ' ';
    }
    std::cout << std::endl;

    auto authlist = ::libssh2_userauth_list(session, gUsername, std::strlen(gUsername));
    if (!authlist) {
        ShowSessionError(session, "Failed to get auth list.");
        return EXIT_FAILURE;
    }

    int authtype = AuthTypeUnknown;
    if (std::strstr(authlist, "password")) {
        ShowInfo("Can use password authentication method.");
        authtype |= AuthTypePassword;
    }
    if (std::strstr(authlist, "keyboard-interactive")) {
        ShowInfo("Can use keyboard-interactive authentication method.");
        authtype |= AuthTypeKeyboardInteractive;
    }
    if (std::strstr(authlist, "publickey")) {
        ShowInfo("Can use publickey authentication method.");
        authtype |= AuthTypePublicKey;
    }

    if (authtype & AuthTypePassword) {
        if (::libssh2_userauth_password(session, gUsername, gPassword)) {
            ShowSessionError(session, "Failed to authenticate by password.");
            return EXIT_FAILURE;
        }
        ShowInfo("Authenticate successfully by using password.");
    } else if (authtype & AuthTypeKeyboardInteractive) {
        if (::libssh2_userauth_keyboard_interactive(session, gUsername, KbdCallback)) {
            ShowSessionError(session, "Failed to authenticate by keyboard.");
            return EXIT_FAILURE;
        }
        ShowInfo("Authenticate successfully by using keyboard-interactive.");
    } else if (authtype & AuthTypePublicKey) {
        if (::libssh2_userauth_publickey_fromfile(session, gUsername, gPublicKeyFile, gPrivateKeyFile, gPassword)) {
            ShowSessionError(session, "Failed to authenticate by public key.");
            return EXIT_FAILURE;
        }
        ShowInfo("Authenticate successfully by using publickey.");
    } else {
        ShowError("Unknown authenticating method.");
        return EXIT_FAILURE;
    }

    // open a generic channel
    auto channel = ::libssh2_channel_open_session(session);
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
    int const ptyWidth  = 120;
    int const ptyHeight = 80;
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

    if (SetNonBlocking(sock)) {
        ShowError(errno);
        return EXIT_FAILURE;
    }
    ::libssh2_channel_set_blocking(channel, 0);

    constexpr int MAXCONN = 2;
    auto epfd = epoll_create(MAXCONN);
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

    ev.data.fd = sock;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev) < 0) {
        ShowError(errno);
        return EXIT_FAILURE;
    }

    bool running = true;
    while (running) {
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
                if (fd == sock) {
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
