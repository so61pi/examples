#include <cstdio>
#include <exception>

#include <sys/sysinfo.h>

#include <clicxx/Editline.h>
#include <clicxx/Handler.h>

class SystemUptimeHandler : public cli::Cmd
{
public:
  explicit SystemUptimeHandler(cli::ArgStaticInfo info)
    : cli::Cmd{ std::move(info), cli::ArgOptions{} }
  {
  }

  virtual int Handle(int /*argc*/, const char** /*argv*/,
                     cli::Editline& editline) override
  {
    struct sysinfo si;
    if (sysinfo(&si)) {
      fprintf(editline.GetIoe().mOutput, "ERROR: Cannot get system uptime.\n");
      return -1;
    } else {
      fprintf(editline.GetIoe().mOutput, "%ld (s)\n", si.uptime);
      return 0;
    }
  }
};

int
main(int /*argc*/, char* /*argv*/ [])
{
  try {
    cli::Editline editline{ cli::IOE{ stdin, stdout, stderr }, nullptr, 100,
                            16 };

    auto cmdRoot_AddChildren = [](cli::Cmd& parent) {
      // exit
      auto cmdExit = cli::MakeCmd<cli::QuitHandler>(
        cli::ArgStaticInfo{
          "exit", "exit the shell", { "exit help 1", "exit help 2" } },
        cli::ArgOptions{});
      parent.AddChildCmd(cmdExit);

      // debug
      auto cmdDebug = cli::MakeCmd<cli::Cmd>(
        cli::ArgStaticInfo{
          "debug", "debug the system", { "debug help 1", "debug help 2" } },
        cli::CmdContainer{}, cli::ArgOptions{ cli::Visibility::Hidden });

      auto cmdDebug_AddChildren = [](cli::Cmd& parent) {
        auto cmdDebugCommand = cli::MakeCmd<cli::CommandHideUnhideHandler>(
          cli::ArgStaticInfo{ "command", "hide or show hidden commands" },
          cli::ArgOptions{});
        parent.AddChildCmd(cmdDebugCommand);

        auto cmdDebugEditline = cli::MakeCmd<cli::EditLineHandler>(
          cli::ArgStaticInfo{ "editline", "libedit related commands" },
          cli::ArgOptions{});
        parent.AddChildCmd(cmdDebugEditline);
      };
      cmdDebug_AddChildren(*cmdDebug);
      parent.AddChildCmd(cmdDebug);

      // system
      auto cmdSystem = cli::MakeCmd<cli::Cmd>(
        cli::ArgStaticInfo{ "system", "system related commands" },
        cli::CmdContainer{}, cli::ArgOptions{});
      auto cmdSystem_AddChildren = [](cli::Cmd& parent) {
        auto cmdSystemUptime = cli::MakeCmd<SystemUptimeHandler>(
          cli::ArgStaticInfo{ "uptime", "display system's uptime" });
        parent.AddChildCmd(cmdSystemUptime);

        auto cmdSystemDisabled = cli::MakeCmd<cli::Cmd>(
          cli::ArgStaticInfo{ "disabled", "disabled command" },
          cli::ArgOptions{ cli::Visibility::Visible,
                           cli::Usability::NotUsable });
        parent.AddChildCmd(cmdSystemDisabled);
      };
      cmdSystem_AddChildren(*cmdSystem);
      parent.AddChildCmd(cmdSystem);

      // help
      auto cmdHelp = cli::MakeCmd<cli::HelpHandler>(
        cli::ArgStaticInfo{ "help", "print help" }, cli::ArgOptions{});
      cmdHelp->AddChildCmd(cmdDebug);
      cmdHelp->AddChildCmd(cmdSystem);
      parent.AddChildCmd(cmdHelp);
    };
    cmdRoot_AddChildren(editline.GetRootCmd());

    editline.Run();
  } catch (std::exception const& e) {
    fprintf(stderr, "EXCEPTION : %s\n", e.what());
    return -1;
  }

  return 0;
}
