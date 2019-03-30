#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <string>
#include <utility>

#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/scope_exit.hpp>

#include <histedit.h>

#include <clicxx/Editline.h>
#include <clicxx/Exception.h>
#include <clicxx/StdEx.h>
#include <clicxx/Utility.h>

#include "Check.h"

namespace cli {
namespace detail {

inline unsigned char
InsertString(EditLine* el, char const* s)
{
  return *s ? (el_insertstr(el, s) ? CC_ERROR : CC_REFRESH) : CC_ARGHACK;
}

inline unsigned char
InsertStringSpace(EditLine* el, char const* s)
{
  if (InsertString(el, s) == CC_ERROR) {
    return CC_ERROR;
  } else {
    return InsertString(el, " ");
  }
}

} // namespace detail

Arg::Arg(ArgStaticInfo info, ArgOptions opts)
  : mInfo{ std::move(info) }
  , mOpts{ opts }
{
}

Arg::~Arg() = default;

std::string const&
Arg::GetName() const
{
  return mInfo.mName;
}

std::string const&
Arg::GetDescription() const
{
  return mInfo.mDesc;
}

Help const&
Arg::GetHelp() const
{
  return mInfo.mHelp;
}

bool
Arg::IsHidden() const
{
  return (mOpts.mVisibility == Visibility::Hidden) || IsNotUsable();
}

bool
Arg::IsVisible() const
{
  return !IsHidden();
}

void
Arg::SetVisibility(Visibility visibility)
{
  mOpts.mVisibility = visibility;
}

bool
Arg::IsUsable() const
{
  return mOpts.mUsability == Usability::Usable;
}

bool
Arg::IsNotUsable() const
{
  return !IsUsable();
}

void
Arg::SetUsability(Usability usability)
{
  mOpts.mUsability = usability;
}

Param::Param(ArgStaticInfo info, ParamCompletor completor,
             std::string defaultHelp, ArgOptions opts)
  : Arg{ std::move(info), opts }
  , mCompletor{ std::move(completor) }
  , mDefaultHelp{ std::move(defaultHelp) }
{
}

CompletionAction
Param::Complete(ValueContainer& vals, stdex::string_view hint) const
{
  CompletionAction action = CompletionAction::Complete;
  if (mCompletor) {
    mCompletor(vals, hint, action);
  } else {
    action = CompletionAction::Help;
  }

  // Cannot display help when there is no help.
  CLI_CHECK(!(action == CompletionAction::Help && mDefaultHelp.empty()));

  return action;
}

std::string const&
Param::GetDefaultHelp() const
{
  return mDefaultHelp;
}

Cmd::Cmd(ArgStaticInfo info, ParamContainer childParams, ArgOptions opts)
  : Arg{ std::move(info), opts }
  , mChildrenKind{ CmdChildrenKind::Parameter }
  , mChildParams{ std::move(childParams) }
{
}

Cmd::Cmd(ArgStaticInfo info, CmdContainer childCmds, ArgOptions opts)
  : Arg{ std::move(info), opts }
  , mChildrenKind{ CmdChildrenKind::Command }
  , mChildCmds{ std::move(childCmds) }
{
}

Cmd::Cmd(ArgStaticInfo info, ArgOptions opts)
  : Arg{ std::move(info), opts }
  , mChildrenKind{ CmdChildrenKind::Nothing }
{
}

CmdChildrenKind
Cmd::GetChildrenKind() const
{
  return mChildrenKind;
}

void
Cmd::AddChildCmd(CmdContainer::value_type cmd)
{
  CLI_CHECK(mChildrenKind == CmdChildrenKind::Command);
  mChildCmds.push_back(std::move(cmd));
}

CmdContainer const&
Cmd::GetChildCmds() const
{
  CLI_CHECK(mChildrenKind == CmdChildrenKind::Command);
  return mChildCmds;
}

CmdContainer&
Cmd::GetChildCmds()
{
  CLI_CHECK(mChildrenKind == CmdChildrenKind::Command);
  return mChildCmds;
}

void
Cmd::AddChildParam(ParamContainer::value_type param)
{
  CLI_CHECK(mChildrenKind == CmdChildrenKind::Parameter);
  mChildParams.push_back(std::move(param));
}

ParamContainer const&
Cmd::GetChildParams() const
{
  CLI_CHECK(mChildrenKind == CmdChildrenKind::Parameter);
  return mChildParams;
}

ParamContainer&
Cmd::GetChildParams()
{
  CLI_CHECK(mChildrenKind == CmdChildrenKind::Parameter);
  return mChildParams;
}

int
Cmd::Handle(int argc, char const** argv, Editline& editline)
{
  CLI_CHECK(argc > 0);
  CLI_CHECK(argv);

  if (mChildrenKind == CmdChildrenKind::Command) {
    if (argc == 1) {
      editline.PrintCmdHelpAndChildren(*this);
      fprintf(editline.GetIoe().mOutput, "\n");
      return -1;
    } else {
      CLI_CHECK(*(argv + 1));
      CmdContainer const& cmds = GetChildCmds();
      auto const pos = std::find_if(
        cmds.begin(), cmds.end(), [&](CmdContainer::value_type const& e) {
          return util::to_string_view(e->GetName()) ==
                 util::string_view(*(argv + 1));
        });
      if (pos == cmds.end() || (*pos)->IsNotUsable()) {
        fprintf(editline.GetIoe().mOutput, "ERROR: Unknown command '%s'\n",
                *(argv + 1));
        return -1;
      }

      return (*pos)->Handle(argc - 1, argv + 1, editline);
    }
  } else {
    // We're at the end of command chain and still cannot find any handler.
    CLI_CHECK(false && "Missing handler");
  }
}

Editline::Editline(IOE ioe, PromptFn prompt, std::size_t histSize,
                   std::size_t maxArgLength)
  : mMaxArgLength{ maxArgLength }
  , mRootCmd{ ArgStaticInfo{ "ROOT", "ROOT", {} }, CmdContainer{},
              ArgOptions{} }
  , mIoe{ ioe }
  , mPrompt{ std::move(prompt) }
  , mCompletionTokenizer{ tok_init(nullptr), tok_end }
  , mHist{ history_init(), history_end }
  , mEvent{}
  , mEditLine{ el_init("null", ioe.mInput, ioe.mOutput, ioe.mError), el_end }
  , mFlagIgnoreLine{ false }
  , mFlagExit{ false }
{
  CLI_RCHECK(mCompletionTokenizer);

  CLI_RCHECK(history(mHist.get(), &mEvent, H_SETSIZE, histSize) == 0);
  CLI_RCHECK(history(mHist.get(), &mEvent, H_SETUNIQUE, 1) == 0);

  // Default key binding is emacs.
  CLI_RCHECK(el_set(mEditLine.get(), EL_EDITOR, "emacs") == 0);

  // Handle signals gracefully.
  CLI_RCHECK(el_set(mEditLine.get(), EL_SIGNAL, 1) == 0);

  // Set the prompt function.
  CLI_RCHECK(el_set(mEditLine.get(), EL_PROMPT, PromptWrapper) == 0);

  // Tell editline to use this history interface.
  CLI_RCHECK(el_set(mEditLine.get(), EL_HIST, history, mHist.get()) == 0);

  // Add a user-defined function & bind tab to it.
  CLI_RCHECK(el_set(mEditLine.get(), EL_ADDFN, "own-ed-complete",
                    "Complete argument", CompleteTabWrapper) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "^I", "own-ed-complete", nullptr) == 0);

  // Ignore current line.
  CLI_RCHECK(el_set(mEditLine.get(), EL_ADDFN, "own-ed-ignore-line",
                    "Ignore current line", IgnoreLineWrapper) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "^C", "own-ed-ignore-line", nullptr) == 0);

  // Allow the use of Home/End keys.
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[1~", "ed-move-to-beg", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[4~", "ed-move-to-end", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[7~", "ed-move-to-beg", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[8~", "ed-move-to-end", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[H", "ed-move-to-beg", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[F", "ed-move-to-end", nullptr) == 0);

  // Allow the use of the Delete/Insert keys.
  CLI_RCHECK(el_set(mEditLine.get(), EL_BIND, "\\e[3~", "ed-delete-next-char",
                    nullptr) == 0);
  CLI_RCHECK(el_set(mEditLine.get(), EL_BIND, "\\e[2~", "ed-quoted-insert",
                    nullptr) == 0);

  // Ctrl-left-arrow and Ctrl-right-arrow for word moving.
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[1;5C", "em-next-word", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[1;5D", "ed-prev-word", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[5C", "em-next-word", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e[5D", "ed-prev-word", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e\\e[C", "em-next-word", nullptr) == 0);
  CLI_RCHECK(
    el_set(mEditLine.get(), EL_BIND, "\\e\\e[D", "ed-prev-word", nullptr) == 0);

  // Don't translate to signals.
  CLI_RCHECK(el_set(mEditLine.get(), EL_SETTY, "-d", "-isig", nullptr) == 0);

  CLI_RCHECK(el_set(mEditLine.get(), EL_CLIENTDATA, static_cast<void*>(this)) ==
             0);
}

Editline::~Editline() = default;

Cmd&
Editline::GetRootCmd()
{
  return mRootCmd;
}

IOE
Editline::GetIoe()
{
  return mIoe;
}

void
Editline::Exit()
{
  mFlagExit = true;
}

void
Editline::ShowArgDesc(bool enable)
{
  mHidingOpts.HideArgDesc = !enable;
}

void
Editline::ShowHiddenArg(bool enable)
{
  mHidingOpts.HideArg = !enable;
}

void
Editline::Run()
{
  while (true) {
    if (mFlagExit) {
      mFlagExit = false;
      return;
    }

    int num = 0;
    auto const line = el_gets(mEditLine.get(), &num);

    if (mFlagIgnoreLine) {
      fprintf(mIoe.mOutput, "\n");
      mFlagIgnoreLine = false;
      continue;
    }

    if (line == nullptr || num == 0) {
      return;
    }

    auto const li = el_line(mEditLine.get());

    int cc = 0;
    int co = 0;
    int argc = 0;
    const char** argv = nullptr;
    int const ncontinuation =
      tok_line(mCompletionTokenizer.get(), li, &argc, &argv, &cc, &co);
    BOOST_SCOPE_EXIT_ALL(&) { tok_reset(mCompletionTokenizer.get()); };
    CLI_RCHECK(ncontinuation >= 0);

    if (ncontinuation > 0) {
      fprintf(mIoe.mOutput, "ERROR: Incomplete command\n");
      continue;
    }

    HandleCommand(line, argc, argv);
  }
}

int
Editline::HandleLine(char const* line, bool& incomplete)
{
  CLI_CHECK(line);

  int argc = 0;
  const char** argv = nullptr;
  auto const ncontinuation =
    tok_str(mCompletionTokenizer.get(), line, &argc, &argv);
  BOOST_SCOPE_EXIT_ALL(&) { tok_reset(mCompletionTokenizer.get()); };
  CLI_RCHECK(ncontinuation >= 0);

  if (ncontinuation > 0) {
    incomplete = true;
    return -1;
  }

  incomplete = false;
  return HandleCommand(line, argc, argv);
}

int
Editline::HandleCommand(char const* line, int argc, char const** argv)
{
  CLI_CHECK(argc >= 0);
  CLI_CHECK(argv);

  if (argc == 0) {
    return 0;
  }

  // If first character is '#', treat current line as a comment.
  if (**argv == '#') {
    return 0;
  }

  if (line) {
    CLI_RCHECK(history(mHist.get(), &mEvent, H_ENTER, line) >= 0);
  }

  auto const first =
    boost::make_indirect_iterator(mRootCmd.GetChildCmds().begin());
  auto const last =
    boost::make_indirect_iterator(mRootCmd.GetChildCmds().end());
  auto const it = std::find_if(first, last, [&](cli::Cmd const& cmd) {
    CLI_CHECK(*argv);
    return util::to_string_view(cmd.GetName()) == util::string_view(*argv);
  });

  if (it == last) {
    fprintf(mIoe.mOutput, "ERROR: Unknown command '%s'\n", *argv);
    return -1;
  }

  return it->Handle(argc, argv, *this);
}

Editline&
Editline::GetThis(EditLine* el)
{
  CLI_CHECK(el);

  Editline* Editline;
  if (el_get(el, EL_CLIENTDATA, &Editline)) {
    throw exception::Logical("EL_CLIENTDATA");
  }

  return *Editline;
}

unsigned char
Editline::IgnoreLineWrapper(EditLine* el, int ch)
{
  CLI_CHECK(el);

  Editline::GetThis(el).IgnoreLine(ch);
  return CC_NEWLINE;
}

unsigned char
Editline::CompleteTabWrapper(EditLine* el, int ch)
{
  return Editline::GetThis(el).CompleteTab(ch);
}

char*
Editline::PromptWrapper(EditLine* el)
{
  return Editline::GetThis(el).GetPrompt();
}

char*
Editline::GetPrompt()
{
  if (mPrompt) {
    return mPrompt();
  }

  static char defprompt[] = "(null) ";
  return defprompt;
}

unsigned char
Editline::IgnoreLine(int /*ch*/)
{
  mFlagIgnoreLine = true;
  return CC_NEWLINE;
}

unsigned char
Editline::CompleteTab(int /*ch*/)
{
  // cursoro  00012300012300
  //         >..abc...def...
  // space    sss   sss   ss
  // cursorc  \____/\____/\_
  //          0     1     2
  //                \____/\_
  //                0     1
  //                      \_
  //                      0
  //            abc   def
  // argv       0     1
  //                  0

  int argc = 0;
  int cursorc = 0;
  int cursoro = 0;
  char const** argv = nullptr;
  auto const li = el_line(mEditLine.get());
  auto const tokstatus =
    tok_line(mCompletionTokenizer.get(), li, &argc, &argv, &cursorc, &cursoro);
  BOOST_SCOPE_EXIT_ALL(&) { tok_reset(mCompletionTokenizer.get()); };

  if (tokstatus < 0) {
    return CC_ERROR;
  } else if (tokstatus == 0) {
    CLI_CHECK(argc >= 0);
    CLI_CHECK(argv);
    CLI_CHECK(cursorc >= 0);
    CLI_CHECK(cursoro >= 0);
    return CompleteCmd(mRootCmd, std::size_t(argc), argv, std::size_t(cursorc),
                       std::size_t(cursoro));
  } else {
    return CC_ARGHACK;
  }
}

template <typename ArgIt>
unsigned char
Editline::CompleteArg(ArgIt first, ArgIt last, stdex::string_view filter)
{
  auto visible = [&](Arg const& arg) {
    return arg.IsUsable() && !(mHidingOpts.HideArg && arg.IsHidden());
  };

  auto transformer = [&](Arg const& arg) {
    std::string name;
    if (arg.GetName().size() < mMaxArgLength) {
      name.resize(mMaxArgLength, ' ');
      std::copy(arg.GetName().begin(), arg.GetName().end(), name.begin());
    } else {
      name = arg.GetName();
    }
    return mHidingOpts.HideArgDesc ? name
                                   : (name + " - " + arg.GetDescription());
  };

  auto const vfirst = boost::make_filter_iterator(visible, first, last);
  auto const vlast = boost::make_filter_iterator(visible, last, last);

  if (std::distance(vfirst, vlast) == 0) {
    return CC_ARGHACK;
  }

  auto const prefix = GetArgCommonPrefix(vfirst, vlast, filter);

  if (prefix.empty()) {
    if (filter.empty()) {
      // All commands don't start with a same prefix.
      fputc('\n', mIoe.mOutput);
      PrintStrings(boost::make_transform_iterator(vfirst, transformer),
                   boost::make_transform_iterator(vlast, transformer));
      fputc('\n', mIoe.mOutput);
      return CC_REDISPLAY;
    } else {
      // No command starts with provided prefix.
      return CC_ARGHACK;
    }
  } else {
    std::size_t cachedSize = 0;
    std::array<Arg const*, 20> cached;

    for (auto it = vfirst; it != vlast; ++it) {
      if (util::start_with(it->GetName(), prefix) &&
          (cachedSize < cached.size())) {
        cached[cachedSize++] = &*it;
      }
    }
    CLI_CHECK(cachedSize > 0);

    if (cachedSize == 1) {
      return detail::InsertStringSpace(
        mEditLine.get(), cached.front()->GetName().c_str() + filter.size());
    } else {
      if (detail::InsertString(
            mEditLine.get(), std::string(prefix.begin(), prefix.end()).c_str() +
                               filter.size()) == CC_ERROR) {
        return CC_ERROR;
      }

      fputc('\n', mIoe.mOutput);
      if (cachedSize <= cached.size()) {
        PrintStrings(
          boost::make_transform_iterator(
            boost::make_indirect_iterator(cached.begin()), transformer),
          boost::make_transform_iterator(
            boost::make_indirect_iterator(cached.begin() + cachedSize),
            transformer));
      } else {
        auto pred = [&](Arg const& arg) {
          return util::start_with(arg.GetName(), prefix);
        };
        PrintStrings(
          boost::make_transform_iterator(
            boost::make_filter_iterator(pred, first, last), transformer),
          boost::make_transform_iterator(
            boost::make_filter_iterator(pred, last, last), transformer));
      }
      fputc('\n', mIoe.mOutput);
      return CC_REDISPLAY;
    }
  }
}

unsigned char
Editline::CompleteCmd(Cmd const& parentcmd, std::size_t argc, char const** argv,
                      std::size_t cursorc, std::size_t cursoro)
{
  if (cursorc > 0) {
    auto const firstarg = *argv;
    switch (parentcmd.GetChildrenKind()) {
      case CmdChildrenKind::Command: {
        CmdContainer const& cmds = parentcmd.GetChildCmds();
        auto const pos = std::find_if(cmds.begin(), cmds.end(),
                                      [&](CmdContainer::value_type const& e) {
                                        return e->GetName() == firstarg;
                                      });
        if (pos == cmds.end() || (*pos)->IsNotUsable()) {
          return CC_ARGHACK;
        } else {
          return CompleteCmd(**pos, argc - 1, argv + 1, cursorc - 1, cursoro);
        }
      }

      case CmdChildrenKind::Parameter: {
        return CompleteParam(parentcmd.GetChildParams(), argc, argv, cursorc,
                             cursoro);
      }

      case CmdChildrenKind::Nothing:
        return CC_ARGHACK;
    }
  } else {
    switch (parentcmd.GetChildrenKind()) {
      case CmdChildrenKind::Command: {
        auto const cursorarg = cursoro ? *argv : nullptr;
        return CompleteArg(
          boost::make_indirect_iterator(parentcmd.GetChildCmds().begin()),
          boost::make_indirect_iterator(parentcmd.GetChildCmds().end()),
          stdex::string_view(cursorarg, cursoro));
      }

      case CmdChildrenKind::Parameter: {
        return CompleteParam(parentcmd.GetChildParams(), argc, argv, cursorc,
                             cursoro);
      }

      case CmdChildrenKind::Nothing:
        return CC_ARGHACK;
    }
  }

  CLI_UNREACHABLE();
}

unsigned char
Editline::CompleteParam(ParamContainer const& params, std::size_t /*argc*/,
                        char const** argv, std::size_t cursorc,
                        std::size_t cursoro)
{
  auto const cursorarg = cursoro ? argv[cursorc] : nullptr;
  if (cursorc % 2 == 0) {
    return CompleteArg(boost::make_indirect_iterator(params.begin()),
                       boost::make_indirect_iterator(params.end()),
                       stdex::string_view(cursorarg, cursoro));
  } else {
    auto const paramname = argv[cursorc - 1];
    auto const pos = std::find_if(params.begin(), params.end(),
                                  [&](ParamContainer::value_type const& e) {
                                    return e->GetName() == paramname;
                                  });
    if (pos == params.end() || (*pos)->IsNotUsable()) {
      return CC_ARGHACK;
    } else {
      return CompleteValue(**pos, stdex::string_view(cursorarg, cursoro));
    }
  }
}

unsigned char
Editline::CompleteValue(Param const& param, stdex::string_view filter)
{
  ValueContainer vals;
  switch (param.Complete(vals, filter)) {
    case CompletionAction::Help: {
      fprintf(mIoe.mOutput, "\n%s\n", param.GetDefaultHelp().c_str());
      return CC_REDISPLAY;
    }

    case CompletionAction::Display: {
      if (vals.empty()) {
        return CC_ARGHACK;
      }

      fputc('\n', mIoe.mOutput);
      PrintStrings(vals.begin(), vals.end());
      fputc('\n', mIoe.mOutput);
      return CC_REDISPLAY;
    }

    case CompletionAction::Complete: {
      auto pred = [&](auto const& sv) { return util::start_with(sv, filter); };

      auto const first =
        boost::make_transform_iterator(vals.begin(), util::to_string_view);
      auto const last =
        boost::make_transform_iterator(vals.end(), util::to_string_view);
      auto const prefix =
        util::string_prefix(boost::make_filter_iterator(pred, first, last),
                            boost::make_filter_iterator(pred, last, last));

      if (prefix.empty()) {
        if (filter.empty()) {
          fputc('\n', mIoe.mOutput);
          PrintStrings(vals.begin(), vals.end());
          fputc('\n', mIoe.mOutput);
          return CC_REDISPLAY;
        } else {
          return CC_ARGHACK;
        }
      } else {
        std::size_t cachedSize = 0;
        std::array<std::string const*, 20> cached;

        for (auto const& val : vals) {
          if (util::start_with(val, filter) && (cachedSize < cached.size())) {
            cached[cachedSize++] = &val;
          }
        }
        CLI_CHECK(cachedSize > 0);

        if (cachedSize == 1) {
          return detail::InsertStringSpace(
            mEditLine.get(), cached.front()->c_str() + filter.size());
        } else {
          if (detail::InsertString(
                mEditLine.get(),
                std::string(prefix.begin(), prefix.end()).c_str() +
                  filter.size()) == CC_ERROR) {
            return CC_ERROR;
          }

          fputc('\n', mIoe.mOutput);
          if (cachedSize <= cached.size()) {
            PrintStrings(
              boost::make_indirect_iterator(cached.begin()),
              boost::make_indirect_iterator(cached.begin() + cachedSize));
          } else {
            PrintStringViews(boost::make_filter_iterator(pred, first, last),
                             boost::make_filter_iterator(pred, last, last));
          }
          fputc('\n', mIoe.mOutput);
          return CC_REDISPLAY;
        }
      }
    }
  }

  CLI_UNREACHABLE();
}

void
Editline::PrintArg(Arg const& arg)
{
  if (mHidingOpts.HideArgDesc) {
    fprintf(mIoe.mOutput, "%s", arg.GetName().c_str());
    return;
  }

  fprintf(mIoe.mOutput, "%-*s - %s", int(mMaxArgLength), arg.GetName().c_str(),
          arg.GetDescription().c_str());
}

template <typename ArgIt>
void
Editline::PrintArgs(ArgIt first, ArgIt last)
{
  bool firstCall = true;
  for (; first != last; ++first) {
    if (firstCall) {
      firstCall = false;
    } else {
      fprintf(mIoe.mOutput, "\n");
    }
    PrintArg(*first);
  }
}

template <typename It>
void
Editline::PrintStrings(It first, It last)
{
  bool firstCall = true;
  for (; first != last; ++first) {
    if (firstCall) {
      firstCall = false;
      fprintf(mIoe.mOutput, "%s", first->c_str());
    } else {
      fprintf(mIoe.mOutput, "\n%s", first->c_str());
    }
  }
}

template <typename It>
void
Editline::PrintStringViews(It first, It last)
{
  bool firstCall = true;
  for (; first != last; ++first) {
    if (firstCall) {
      firstCall = false;
      fprintf(mIoe.mOutput, "%*s", int(first->size()), first->data());
    } else {
      fprintf(mIoe.mOutput, "\n%*s", int(first->size()), first->data());
    }
  }
}

void
Editline::PrintHelp(Help const& help)
{
  bool firstCall = true;
  for (auto const& line : help) {
    if (firstCall) {
      firstCall = false;
      fprintf(mIoe.mOutput, "%s", line.c_str());
    } else {
      fprintf(mIoe.mOutput, "\n%s", line.c_str());
    }
  }
}

void
Editline::PrintCmdHelpAndChildren(Cmd const& cmd)
{
  // Temporarily change HideArgDesc to make sure that the child commands or
  // parameters will have desc displayed.
  auto const savedHideArgDesc = mHidingOpts.HideArgDesc;
  mHidingOpts.HideArgDesc = false;
  BOOST_SCOPE_EXIT_ALL(&) { mHidingOpts.HideArgDesc = savedHideArgDesc; };

  bool firstCall = true;

  if (cmd.GetDescription().empty() == false) {
    fprintf(mIoe.mOutput, "%s", cmd.GetDescription().c_str());
    firstCall = false;
  }

  if (cmd.GetHelp().empty() == false) {
    if (firstCall) {
      firstCall = false;
    } else {
      fprintf(mIoe.mOutput, "\n\n");
    }
    PrintHelp(cmd.GetHelp());
  }

  auto createIterators = [&](auto const& container) {
    auto visible = [&](Arg const& arg) {
      return !(arg.IsHidden() && mHidingOpts.HideArg);
    };
    auto const first = boost::make_indirect_iterator(container.begin());
    auto const last = boost::make_indirect_iterator(container.end());
    return std::make_pair(boost::make_filter_iterator(visible, first, last),
                          boost::make_filter_iterator(visible, last, last));
  };

  switch (cmd.GetChildrenKind()) {
    case CmdChildrenKind::Command: {
      auto const its = createIterators(cmd.GetChildCmds());
      if (std::distance(its.first, its.second) > 0) {
        if (firstCall) {
          firstCall = false;
          fprintf(mIoe.mOutput, "Commands:\n");
        } else {
          fprintf(mIoe.mOutput, "\n\nCommands:\n");
        }
        PrintArgs(its.first, its.second);
      }
      break;
    }

    case CmdChildrenKind::Parameter: {
      auto const its = createIterators(cmd.GetChildParams());
      if (std::distance(its.first, its.second) > 0) {
        if (firstCall) {
          firstCall = false;
          fprintf(mIoe.mOutput, "Parameters:\n");
        } else {
          fprintf(mIoe.mOutput, "\n\nParameters:\n");
        }
        PrintArgs(its.first, its.second);
      }
      break;
    }

    case CmdChildrenKind::Nothing:
      break;
  }
}

template <typename ArgIt>
stdex::string_view
Editline::GetArgCommonPrefix(ArgIt first, ArgIt last, stdex::string_view filter)
{
  auto pred = [&](Arg const& arg) {
    return util::start_with(arg.GetName(), filter);
  };

  auto sv = [](Arg const& arg) { return util::to_string_view(arg.GetName()); };

  return util::string_prefix(
    boost::make_transform_iterator(
      boost::make_filter_iterator(pred, first, last), sv),
    boost::make_transform_iterator(
      boost::make_filter_iterator(pred, last, last), sv));
}

} // namespace cli
