#include "Expect.h"

#include <cassert>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <exception>
#include <iostream>
#include <iterator>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>

#include <poll.h>
#include <pty.h>
#include <unistd.h>


auto expect::Status::ToString(expect::Status::type value) -> std::string {
    switch (value) {
    case Default:
        return "Status::Default";
    case Timeout:
        return "Status::Timeout";
    case Success:
        return "Status::Success";
    case Error:
        return "Status::Error";
    default:
        return "Invalid Status value.";
    }
}


expect::String::String(std::string const& str, bool regex,
                       std::regex_constants::syntax_option_type reopt)
    : base_type{str}, m_regex{regex}, m_reopt{reopt} {}


auto expect::detail::search(std::string const& haystack, expect::String const& needle) -> bool {
    if (needle.m_regex) {
        return std::regex_search(haystack, std::regex{needle});
    } else {
        return haystack.find(needle) != std::string::npos;
    }
}


expect::ExpectProcess::ExpectProcess(std::string const&    path,
                                     char* const           argv[],
                                     expect::Option const& opt)
    : m_opt{opt} {
    m_childPid = forkpty(&m_masterPtyFd, nullptr, nullptr, nullptr);
    if (m_childPid < 0) Throw(errno);

    if (m_childPid == 0) {
        // child
        execvp(path.c_str(), argv);
    } else {
        // parent
    }
}


expect::ExpectProcess::~ExpectProcess() {
    close(m_masterPtyFd);
    kill(m_childPid, SIGKILL);
}


//
//
//
auto expect::ExpectProcess::Send(std::string const& cmd) -> Status::type {
    SaveCommand(cmd);
    if (cmd.empty()) return Status::Success;

    auto size = static_cast<ssize_t>(cmd.size());
    auto data = cmd.c_str();

    while (true) {
        auto bytes = write(m_masterPtyFd, data, size);
        if (bytes > size) {
            Throw("Insane error.");
        } else if (bytes == size) {
            break;
        } else if (bytes > 0) {
            data += bytes;
            size -= bytes;
        } else {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            } else {
                return Status::Error;
            }
        }
    }
    return Status::Success;
}


auto expect::ExpectProcess::Expect(std::vector<String> const& texts)
    -> Status::type {
    auto pos = Status::Default;
    auto stop       = [&](auto const& lastOutput) {
        for (auto i = 0U; i < texts.size(); ++i) {
            if (detail::search(lastOutput, texts[i])) {
                pos = static_cast<Status::type>(i);
                return true;
            }
        }
        return false;
    };
    auto status = ReadPty(stop);
    if (pos != Status::Default) {
        return pos;
    } else {
        return status;
    }
}


auto expect::ExpectProcess::Expect(String const& text) -> Status::type {
    std::vector<String> texts{text};
    return Expect(texts);
}


auto expect::ExpectProcess::Expect(String const&      text,
                                   std::string const& action) -> Status::type {
    auto stop = [&](auto const& lastOutput) {
        if (detail::search(lastOutput, text)) {
            Send(action);
            return true;
        }
        return false;
    };
    return ReadPty(stop);
}


template <typename Fn>
auto expect::ExpectProcess::ReadPty(Fn fnAfterReadChar) -> typename Status::type {
    pollfd fds = {m_masterPtyFd, POLLIN, 0};
    m_lastOutput.clear();
    auto status = poll(&fds, 1, m_opt.Timeout());
    if (status < 0) {
        Log(errno);
        return Status::Error;
    } else if (status == 0) {
        Log("Expect timeout.");
        return Status::Timeout;
    }

    if (fds.revents != POLLIN) {
        // we only have 1 fd, so if it's not POLLIN,
        // there must be something wrong
        Log("Expect ERROR.");
        return Status::Error;
    }

    while (true) {
        char output;
        auto bytes = read(m_masterPtyFd, &output, 1);
        if (bytes > 0) {
            m_lastOutput.push_back(output);
            Capture(output);
            if (fnAfterReadChar(m_lastOutput)) {
                return Status::Success;
            }
        } else {
            switch (errno) {
            case EINTR:
                continue;

            case EAGAIN:
                return Status::Default;

            default:
                Log(errno);
                return Status::Error;
            }
        }
    }
}


//
//
//
auto expect::ExpectProcess::ReadLast() -> Status::type {
    return ReadPty([](auto const&) { return false; });
}


//
//
//
auto expect::ExpectProcess::LastOutput() const -> std::string const& {
    return m_lastOutput;
}


//
//
//
void expect::ExpectProcess::StartCaptureOutput() {
    m_opt.CaptureOutput(true);
    ClearCapturedOutput();
}

void expect::ExpectProcess::StopCaptureOutput() { m_opt.CaptureOutput(false); }

auto expect::ExpectProcess::CapturedOutput() const -> std::string const& {
    return m_capturedOutput;
}

void expect::ExpectProcess::ClearCapturedOutput() { m_capturedOutput.clear(); }

void expect::ExpectProcess::Capture(char ch) {
    if (m_opt.CaptureOutput()) m_capturedOutput.push_back(ch);
}


//
//
//
auto expect::ExpectProcess::AllSentCommands() const
    -> std::vector<std::string> const& {
    return m_sentCommands;
}

void expect::ExpectProcess::SaveCommand(std::string const& cmd) {
    if (m_opt.SaveCommand()) m_sentCommands.emplace_back(cmd);
}


//
//
//
void expect::ExpectProcess::SetTimeout(int ms) { m_opt.Timeout(ms); }

auto expect::ExpectProcess::GetOption() const -> Option const& { return m_opt; }
void expect::ExpectProcess::SetOption(Option const& opt) { m_opt = opt; }


//
// Log and Exception
//
void expect::ExpectProcess::Log(std::string const& msg) {
    if (m_opt.Log()) std::cerr << "Expect log : " << msg << "\n";
}

void expect::ExpectProcess::Log(int errnum) { Log(std::strerror(errnum)); }

void expect::ExpectProcess::Throw(std::string const& cmd) {
    throw std::runtime_error{"Expect exception : " + cmd};
}

void expect::ExpectProcess::Throw(int errnum) { Throw(std::strerror(errnum)); }
