#ifndef LIBCLICXX_HANDLER_H_INCLUDED
#define LIBCLICXX_HANDLER_H_INCLUDED

#include <clicxx/Editline.h>

namespace cli {

class QuitHandler : public Cmd
{
public:
  explicit QuitHandler(ArgStaticInfo info, ArgOptions opts);

  virtual int Handle(int argc, char const** argv, Editline& editline) override;
};

class CommandHideUnhideHandler : public Cmd
{
public:
  explicit CommandHideUnhideHandler(ArgStaticInfo info, ArgOptions opts);

  virtual int Handle(int argc, char const** argv, Editline& editline) override;
};

class HelpHandler : public Cmd
{
public:
  explicit HelpHandler(ArgStaticInfo info, ArgOptions opts);

  virtual int Handle(int argc, char const** argv, Editline& editline) override;
};

class EditLineHandler : public Cmd
{
public:
  explicit EditLineHandler(ArgStaticInfo info, ArgOptions opts);

  virtual int Handle(int argc, char const** argv, Editline& editline) override;
};

} // namespace cli

#endif // LIBCLICXX_HANDLER_H_INCLUDED
