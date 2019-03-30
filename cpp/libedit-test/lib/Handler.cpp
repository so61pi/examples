#include <algorithm>
#include <cstring>

#include <boost/iterator/indirect_iterator.hpp>

#include <clicxx/Handler.h>
#include <clicxx/Utility.h>

#include "Check.h"

namespace cli {

QuitHandler::QuitHandler(ArgStaticInfo info, ArgOptions opts)
  : Cmd{ std::move(info), opts }
{
}

int
QuitHandler::Handle(int /*argc*/, char const** /*argv*/, Editline& editline)
{
  editline.Exit();
  return 0;
}

CommandHideUnhideHandler::CommandHideUnhideHandler(ArgStaticInfo info,
                                                   ArgOptions opts)
  : Cmd{ std::move(info), CmdContainer{}, opts }
{
  AddChildCmd(MakeCmd<Cmd>(
    ArgStaticInfo{ "hide-command", "hide all hidden commands" }, ArgOptions{}));
  AddChildCmd(MakeCmd<Cmd>(
    ArgStaticInfo{ "unhide-command", "unhide all hidden commands" },
    ArgOptions{}));

  AddChildCmd(MakeCmd<Cmd>(ArgStaticInfo{ "hide-desc", "hide command description" },
                      ArgOptions{}));
  AddChildCmd(
    MakeCmd<Cmd>(ArgStaticInfo{ "unhide-desc", "unhide command description" },
                 ArgOptions{}));
}

int
CommandHideUnhideHandler::Handle(int argc, char const** argv,
                                 Editline& editline)
{
  if (argc == 1) {
    editline.PrintCmdHelpAndChildren(*this);
    fprintf(editline.GetIoe().mOutput, "\n");
    return -1;
  }

  if (std::strcmp("hide-command", argv[1]) == 0) {
    editline.ShowHiddenArg(false);
  } else if (std::strcmp("unhide-command", argv[1]) == 0) {
    editline.ShowHiddenArg(true);
  } else if (std::strcmp("hide-desc", argv[1]) == 0) {
    editline.ShowArgDesc(false);
  } else if (std::strcmp("unhide-desc", argv[1]) == 0) {
    editline.ShowArgDesc(true);
  } else {
    CLI_UNREACHABLE();
  }
  return 0;
}

HelpHandler::HelpHandler(ArgStaticInfo info, ArgOptions opts)
  : Cmd{ std::move(info), CmdContainer{}, opts }
{
}

int
HelpHandler::Handle(int argc, char const** argv, Editline& editline)
{
  Cmd const* lastcmd = this;
  if (argc > 1) {
    int pos = 0;
    for (int i = 1; i < argc; ++i) {
      if (lastcmd->GetChildrenKind() != cli::CmdChildrenKind::Command) {
        break;
      }

      cli::CmdContainer const& childcmds = lastcmd->GetChildCmds();
      auto first = boost::make_indirect_iterator(childcmds.begin());
      auto last = boost::make_indirect_iterator(childcmds.end());
      auto it = std::find_if(first, last, [&](cli::Cmd const& cmd) {
        return util::to_string_view(cmd.GetName()) ==
               util::to_string_view(argv[i]);
      });
      if (it == last) {
        fprintf(editline.GetIoe().mOutput, "ERROR: Unknown command '%s'\n",
                argv[i]);
        return 0;
      } else {
        lastcmd = &*it;
        pos = i;
      }
    }

    fprintf(editline.GetIoe().mOutput, "[");
    bool firstCall = true;
    for (auto it = argv + 1; it != argv + pos + 1; ++it) {
      if (firstCall) {
        firstCall = false;
        fprintf(editline.GetIoe().mOutput, "%s", *it);
      } else {
        fprintf(editline.GetIoe().mOutput, " %s", *it);
      }
    }
    fprintf(editline.GetIoe().mOutput, "]\n");
  }
  editline.PrintCmdHelpAndChildren(*lastcmd);
  fprintf(editline.GetIoe().mOutput, "\n");

  return 0;
}

EditLineHandler::EditLineHandler(ArgStaticInfo info, ArgOptions opts)
  : Cmd{ std::move(info), CmdContainer{}, opts }
{
  AddChildCmd(
    MakeCmd<Cmd>(ArgStaticInfo{ "history", "display history" }, ArgOptions{}));
  AddChildCmd(
    MakeCmd<Cmd>(ArgStaticInfo{ "setty", "list tty modes" }, ArgOptions{}));
  AddChildCmd(MakeCmd<Cmd>(ArgStaticInfo{ "telltc", "list terminal capabilities" },
                      ArgOptions{}));
}

int
EditLineHandler::Handle(int argc, char const** argv, Editline& editline)
{
  if (argc > 1) {
    el_parse(editline.mEditLine.get(), argc - 1, argv + 1);
  }
  return 0;
}

} // namespace cli
