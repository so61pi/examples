#ifndef LIBCLICXX_EDITLINE_H_INCLUDED
#define LIBCLICXX_EDITLINE_H_INCLUDED

#include <cstddef>
#include <cstdio>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <histedit.h>

#include <clicxx/StdEx.h>

namespace cli {
namespace detail {

struct HidingOptions
{
  HidingOptions()
    : HideArgDesc(false)
    , HideArg(true)
  {
  }

  bool HideArgDesc;
  bool HideArg;
};

} // namespace detail

enum class Visibility : bool
{
  Hidden,
  Visible
};

enum class Usability : bool
{
  NotUsable,
  Usable
};

using Help = std::vector<std::string>;

struct ArgStaticInfo
{
  explicit ArgStaticInfo(std::string name, std::string desc)
    : mName(std::move(name))
    , mDesc(std::move(desc))
  {
  }

  explicit ArgStaticInfo(std::string name, std::string desc, Help help)
    : mName(std::move(name))
    , mDesc(std::move(desc))
    , mHelp(std::move(help))
  {
  }

  std::string const mName;
  std::string const mDesc;
  cli::Help const mHelp;
};

struct ArgOptions
{
  ArgOptions() = default;

  explicit ArgOptions(Visibility v)
    : mVisibility(v)
  {
  }

  explicit ArgOptions(Usability u)
    : mUsability(u)
  {
  }

  explicit ArgOptions(Visibility v, Usability u)
    : mVisibility(v)
    , mUsability(u)
  {
  }

  Visibility mVisibility = Visibility::Visible;
  Usability mUsability = Usability::Usable;
};

class Arg
{
public:
  Arg(Arg const&) = delete;
  Arg(Arg&&) = delete;
  Arg& operator=(Arg const&) = delete;
  Arg& operator=(Arg&&) = delete;

public:
  explicit Arg(ArgStaticInfo info, ArgOptions opts);

  virtual ~Arg() = 0;

  std::string const& GetName() const;
  std::string const& GetDescription() const;
  Help const& GetHelp() const;

  bool IsHidden() const;
  bool IsVisible() const;
  void SetVisibility(Visibility visibility);

  bool IsUsable() const;
  bool IsNotUsable() const;
  void SetUsability(Usability usability);

private:
  ArgStaticInfo mInfo;
  ArgOptions mOpts;
};

enum class CompletionAction
{
  Complete, //!< Complete input.
  Display,  //!< Display all values without completing.
  Help      //!< Display help.
};

using ValueContainer = std::vector<std::string>;
using ParamCompletor = std::function<void(
  ValueContainer& vals, stdex::string_view hint, CompletionAction& action)>;

class Param : public Arg
{
public:
  explicit Param(ArgStaticInfo info, ParamCompletor completor,
                 std::string defaultHelp, ArgOptions opts);

  CompletionAction Complete(ValueContainer& vals,
                            stdex::string_view hint) const;

  std::string const& GetDefaultHelp() const;

private:
  ParamCompletor mCompletor;
  std::string mDefaultHelp;
};

using ParamPtr = std::unique_ptr<Param>;
using ParamContainer = std::vector<ParamPtr>;

template <typename T, typename... Args>
inline ParamPtr
MakeParam(Args&&... args)
{
  return std::make_unique<T>(std::forward<Args>(args)...);
}

enum class CmdChildrenKind
{
  Nothing,
  Command,
  Parameter
};

struct IOE
{
  explicit IOE(FILE* i, FILE* o, FILE* e)
    : mInput(i)
    , mOutput(o)
    , mError(e)
  {
  }

  FILE* mInput;
  FILE* mOutput;
  FILE* mError;
};

class Editline;
class Cmd;

using CmdPtr = std::shared_ptr<Cmd>;
using CmdContainer = std::vector<CmdPtr>;

class Cmd : public Arg
{
public:
  explicit Cmd(ArgStaticInfo info, ParamContainer childParams, ArgOptions opts);
  explicit Cmd(ArgStaticInfo info, CmdContainer childCmds, ArgOptions opts);
  explicit Cmd(ArgStaticInfo info, ArgOptions opts);

  virtual ~Cmd() = default;

  CmdChildrenKind GetChildrenKind() const;

  void AddChildCmd(CmdContainer::value_type cmd);
  CmdContainer const& GetChildCmds() const;
  CmdContainer& GetChildCmds();

  void AddChildParam(ParamContainer::value_type param);
  ParamContainer const& GetChildParams() const;
  ParamContainer& GetChildParams();

  virtual int Handle(int argc, char const** argv, Editline& editline);

private:
  CmdChildrenKind mChildrenKind;
  CmdContainer mChildCmds;
  ParamContainer mChildParams;
};

template <typename T, typename... Args>
inline CmdPtr
MakeCmd(Args&&... args)
{
  return std::make_shared<T>(std::forward<Args>(args)...);
}

using PromptFn = std::function<char*(void)>;

class Editline
{
private:
  static Editline& GetThis(EditLine* el);

  static unsigned char IgnoreLineWrapper(EditLine* el, int ch);
  static unsigned char CompleteTabWrapper(EditLine* el, int ch);
  static char* PromptWrapper(EditLine* el);

  // friendzone
  friend class EditLineHandler;

public:
  explicit Editline(IOE ioe, PromptFn prompt, std::size_t histSize,
                    std::size_t maxArgLength);
  ~Editline();

  Cmd& GetRootCmd();

  IOE GetIoe();

  void Exit();
  void ShowArgDesc(bool enable);
  void ShowHiddenArg(bool enable);

  void Run();

  int HandleLine(char const* line, bool& incomplete);
  int HandleCommand(char const* line, int argc, char const** argv);

private:
  char* GetPrompt();
  unsigned char IgnoreLine(int ch);
  unsigned char CompleteTab(int ch);

  template <typename ArgIt>
  unsigned char CompleteArg(ArgIt first, ArgIt last, stdex::string_view filter);
  unsigned char CompleteCmd(Cmd const& parentcmd, std::size_t argc,
                            char const** argv, std::size_t cursorc,
                            std::size_t cursoro);
  unsigned char CompleteParam(ParamContainer const& params, std::size_t argc,
                              char const** argv, std::size_t cursorc,
                              std::size_t cursoro);
  unsigned char CompleteValue(Param const& param, stdex::string_view filter);

private:
  void PrintArg(Arg const& arg);

  template <typename ArgIt>
  void PrintArgs(ArgIt first, ArgIt last);

  template <typename It>
  void PrintStrings(It first, It last);

  template <typename It>
  void PrintStringViews(It first, It last);

  void PrintHelp(Help const& help);

public:
  void PrintCmdHelpAndChildren(Cmd const& cmd);

private:
  template <typename ArgIt>
  stdex::string_view GetArgCommonPrefix(ArgIt first, ArgIt last,
                                        stdex::string_view filter);

private:
  std::size_t const mMaxArgLength;

  Cmd mRootCmd;
  IOE mIoe;
  PromptFn mPrompt;

  std::unique_ptr<Tokenizer, decltype(&tok_end)> mCompletionTokenizer;
  std::unique_ptr<History, decltype(&history_end)> mHist;
  HistEvent mEvent;
  std::unique_ptr<EditLine, decltype(&el_end)> mEditLine;

  detail::HidingOptions mHidingOpts;

  bool mFlagIgnoreLine;
  bool mFlagExit;
};

} // namespace cli

#endif // LIBCLICXX_EDITLINE_H_INCLUDED
