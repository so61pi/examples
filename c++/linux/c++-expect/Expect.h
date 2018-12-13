#ifndef EXPECT_H
#define EXPECT_H


#include <cstddef>
#include <regex>
#include <string>


namespace expect {
    ///
    /// \brief Return code for some ExpectProcess member functions.
    ///
    struct Status {
        using type = std::size_t;
        static constexpr type Default = 1'000'000;
        static constexpr type Timeout = 1'000'001;
        static constexpr type Success = 1'000'002;
        static constexpr type Error   = 1'000'003;

        static auto ToString(type value) -> std::string;
    };


    class String;
    namespace detail {
        auto search(std::string const& haystack, expect::String const& needle) -> bool;
    }


    ///
    /// \brief Hold string with regex option.
    ///
    class String : std::string {
    public:
        using base_type = std::string;
        using base_type::base_type;

        explicit String(std::string const& str, bool regex,
                        std::regex_constants::syntax_option_type reopt =
                            std::regex::ECMAScript);

        friend auto detail::search(std::string const& haystack,
                                   String const&      needle) -> bool;

    private:
        bool                                     m_regex = false;
        std::regex_constants::syntax_option_type m_reopt;
    };


    ///
    /// Use re to quickly create a regex string.
    /// auto restr = "[0-9]{10}" + re;
    /// auto other = "[0-9]{10}" + re(std::regex::ECMAScript);
    ///
    namespace detail {
        struct regex_tag_t {
            regex_tag_t() = default;
            regex_tag_t(std::regex_constants::syntax_option_type reopt)
                : m_reopt{reopt} {}

            regex_tag_t operator()(
                std::regex_constants::syntax_option_type reopt) {
                return regex_tag_t{reopt};
            }

            std::regex_constants::syntax_option_type m_reopt =
                std::regex::ECMAScript;
        };

        inline auto operator+(regex_tag_t const& re, std::string const& str) {
            return String(str, true, re.m_reopt);
        }

        inline auto operator+(std::string const& str, regex_tag_t const& re) {
            return re + str;
        }
    }


    [[maybe_unused]]
    static detail::regex_tag_t re;


    ///
    /// \brief Contain all ExpectProcess options.
    ///
    class Option {
    public:
        inline void Log(bool val) { m_log = val; }
        inline auto Log() const { return m_log; }

        inline void SaveCommand(bool val) { m_saveCommand = val; }
        inline auto SaveCommand() const { return m_saveCommand; }

        inline void Timeout(int val) { m_timeout = val; }
        inline auto Timeout() const { return m_timeout; }

        inline void CaptureOutput(bool val) { m_captureOutput = val; }
        inline auto CaptureOutput() const { return m_captureOutput; }

    private:
        bool m_log           = true;
        bool m_saveCommand   = false;
        int  m_timeout       = 10000; // ms
        bool m_captureOutput = false;
    };


    ///
    /// \brief The ExpectProcess class
    ///
    class ExpectProcess {
    public:
        explicit ExpectProcess(std::string const& path, char* const argv[], Option const& opt = {});

        ExpectProcess(ExpectProcess const&) = delete;
        ExpectProcess(ExpectProcess&&) = delete;
        auto operator=(ExpectProcess const&) -> ExpectProcess& = delete;
        auto operator=(ExpectProcess&&) -> ExpectProcess& = delete;

        ~ExpectProcess();

        auto Send(std::string const& cmd) -> Status::type;
        auto Expect(std::vector<String> const& texts) -> Status::type;
        auto Expect(String const& text) -> Status::type;
        auto Expect(String const& text, std::string const& action) -> Status::type;
        auto ReadLast() -> Status::type;
        auto LastOutput() const -> std::string const&;

    private:
        template <typename Fn>
        auto ReadPty(Fn fnAfterReadChar) -> typename Status::type;

    public:
        void StartCaptureOutput();
        void StopCaptureOutput();
        auto CapturedOutput() const -> std::string const&;
        void ClearCapturedOutput();
    private:
         void Capture(char ch);
    private:
        std::string m_capturedOutput;

    public:
        auto AllSentCommands() const -> std::vector<std::string> const&;
    private:
        void SaveCommand(std::string const& cmd);
    private:
        std::vector<std::string> m_sentCommands;

    public:
        void SetTimeout(int ms);
        auto GetOption() const -> Option const&;
        void SetOption(Option const& opt);

    private:
        void Log(std::string const& msg);
        void Log(int errnum);
        void Throw(std::string const& cmd);
        void Throw(int errnum);

    private:
        Option m_opt;

        pid_t m_childPid    = -1;
        int   m_masterPtyFd = -1;

        std::string m_lastOutput;
    };

} // namespace expect


#endif // EXPECT_H
